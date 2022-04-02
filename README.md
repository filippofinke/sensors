<h1 align="center">Welcome to sensors 👋</h1>

> Simple sensor data ingestion server

## Install

```sh
npm install
```

## Usage

```sh
npm run start
```

### Env variables

| Variable | Description            | Default |
| -------- | ---------------------- | ------- |
| PORT     | Server port            | 8080    |
| KEY      | Authentication key     | -       |
| URI      | MongoDB connection URI | -       |

## Ingest data

You must send a `post` request to `/` with the `X-API-Key` header containing the following JSON structure:

```json
{
    "name": "SENSOR_NAME",
    "value": SENSOR_VALUE,
    "unit": "SENSOR_UNIT"
}
```

## Author

👤 **Filippo Finke**

- Website: https://filippofinke.ch
- Twitter: [@filippofinke](https://twitter.com/filippofinke)
- Github: [@filippofinke](https://github.com/filippofinke)
- LinkedIn: [@filippofinke](https://linkedin.com/in/filippofinke)

## 🤝 Contributing

Contributions, issues and feature requests are welcome!<br />Feel free to check [issues page](https://github.com/filippofinke/sensors/issues).

## Show your support

Give a ⭐️ if this project helped you!

<a href="https://www.buymeacoffee.com/filippofinke">
  <img src="https://github.com/filippofinke/filippofinke/raw/main/images/buymeacoffe.png" alt="Buy Me A McFlurry">
</a>
