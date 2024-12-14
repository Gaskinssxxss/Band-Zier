const mongoose = require("mongoose");
const Schema = mongoose.Schema;

const BanjirSchema = new Schema({
  sensor: {
    require: true,
    type: Number,
  },
  timestamp: {
    type: Date,
    default: Date.now,
  },
});

const Banjir = mongoose.model("Banjir", BanjirSchema);
module.exports = Banjir;
