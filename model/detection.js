const mongoose = require("mongoose");
const Schema = mongoose.Schema;

const DetectionSchema = new Schema({
  sensor: { type: Number, required: true },
  timestamp: { type: Date, default: Date.now },
  status: { type: String, default: "Normal" }, // Default status "Normal"
});

const Detection = mongoose.model("Detection", DetectionSchema);
module.exports = Detection;
