module.exports = (req, res, next) => {
  const key = req.headers["x-api-key"];

  if (key !== process.env.KEY) {
    return res.status(401).send("Unauthorized");
  }

  return next();
};
