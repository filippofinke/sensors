process.env.PORT = process.env.PORT || 8080;

if (!process.env.KEY) {
  console.log("No API key found. Please set the KEY environment variable.");
  process.exit(1);
}

if (!process.env.URI) {
  console.log("No URI found. Please set the URI environment variable.");
  process.exit(1);
}

const { name, version } = require("./package");
const express = require("express");
const morgan = require("morgan");
const ajv = new (require("ajv"))();
const { MongoClient } = require("mongodb");

const schema = require("./src/schema");
const authMiddleware = require("./src/authMiddleware");

const client = new MongoClient(process.env.URI, {
  serverSelectionTimeoutMS: 1000,
});
const app = express();

let measurements = null;

app.use(authMiddleware);

app.use(express.json());
app.use(morgan("dev"));

app.post("/", (req, res) => {
  let data = req.body;

  if (!Array.isArray(data)) {
    data = [data];
  }

  let date = new Date();

  for (let measurement of data) {
    if (!ajv.validate(schema, measurement)) {
      return res.status(400).send(ajv.errorsText());
    }
    measurement.date = date;
  }

  measurements.insertMany(data);

  return res.sendStatus(201);
});

console.log(`${name} v${version} connecting to mongodb...`);
client.connect((err) => {
  if (err) {
    console.log(err.message);
    process.exit(1);
  }

  measurements = client.db().collection("measurements");

  measurements;

  console.log(`${name} v${version} connected to mongodb`);
  app.listen(process.env.PORT, () => {
    console.log(`${name} v${version} listening on port ${process.env.PORT}`);
  });
});
