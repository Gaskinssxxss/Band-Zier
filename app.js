require("dotenv").config();
const express = require("express");
const morgan = require("morgan");
const mongoose = require("mongoose");
const cors = require("cors");

const Detection = require("./model/detection");
const app = express();

app.use(cors());
app.use(morgan("dev"));
app.use(express.json());
app.use(express.urlencoded({ extended: true }));

mongoose
  .connect(process.env.DB_URI, {
    useNewUrlParser: true,
    useUnifiedTopology: true,
  })
  .then(() => console.log("INFO - MongoDB connected successfully."))
  .catch((err) => console.log(`ERROR - MongoDB not connected : ${err} `));

app.post("/api/sensor", async (req, res) => {
  const { sensor } = req.body;

  try {
    const initialData = await Detection.findOne().sort({ timestamp: 1 });
    const latestData = await Detection.findOne().sort({ timestamp: -1 });

    let status = "Normal";

    if (initialData) {
      const initialDistance = initialData.sensor;
      const currentDistance = sensor;

      const percentageChange =
        ((initialDistance - currentDistance) / initialDistance) * 100;

      if (percentageChange >= 50 && percentageChange < 75) {
        status = "Flood Warning";
      }
      if (percentageChange >= 75) {
        status = "Flood Alert";
      }
    }

    const newData = new Detection({ sensor, status });
    const savedData = await newData.save();

    res.status(201).json(savedData);
  } catch (error) {
    res.status(400).json({ message: "Error saving data", error });
  }
});

app.get("/api/sensor", async (req, res) => {
  try {
    const respons = await Detection.find();
    res.json(respons);
  } catch (error) {
    res.status(500).json({ message: "Error fetching data" });
  }
});

app.get("/api/sensor/status", async (req, res) => {
  try {
    const latestData = await Detection.findOne().sort({ timestamp: -1 });

    if (latestData) {
      res.json({
        sensor: `${latestData.sensor.toFixed(2)}`,
        status: latestData.status,
        timestamp: latestData.timestamp,
      });
    } else {
      res.json({ status: "No Data", sensor: null });
    }
  } catch (error) {
    res.status(500).json({ message: "Error fetching status", error });
  }
});

const port = process.env.PORT || 3000;
app.listen(port, () => {
  console.log(`Server is listening on port ${port}`);
});
