### Arduino-sketsj for nodemcu-temperatursensor

Denne koden kan flashes til et nodemcu esp8266-brett for å sende temperatur- og fuktighetsdata over MQTT til en MQTT-server.

Koden kan enkelt endres til å benytte seg av enten DHT11 eller DHT22 temperatur-sensorene.

For å ta koden i bruk trenger man å vite ip-adressen til mqtt-serveren, samt brukernavn og passord til denne. Man trenger også å opprette en unik id for brettet som kjører koden. Det kan være en enkel tekststreng som unikt identifiserer brettet på mqtt-serveren.

##### Typiske feilkilder

- Feil type usb-kabel som ikke kan overføre data. Det finnes ulike typer usb-kabler og noen kan ikke sende data.
- Man har glemt å endre `clientId`. Dette skaper problemer hvis man har to klienter som går mot samme mqtt-server med samme id. Da tror serveren av det finnes en tilkobling fra før og disconnecter annenhvert kall.
- Ledninger som ikke er godt nok tilkoblet sensoren. Typisk sender programmet da NaN-verdier. Kan være vanskelig å feilsøke, men dobbeltsjekk at alle kontakter er korrekt.
