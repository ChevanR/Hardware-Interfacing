#include <Servo.h>
 
#define TRIG_PIN 3    // Pin verbonden met de TRIG pin van de ultrasone sensor
#define ECHO_PIN 4    // Pin verbonden met de ECHO pin van de ultrasone sensor
#define SERVO_PIN 9   // Pin verbonden met het besturingssignaal van de servomotor
 
Servo servoMotor;
int initialPosition = 180; // Initial position of the servo motor
 
void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  servoMotor.attach(SERVO_PIN);
  servoMotor.write(initialPosition); // Move servo to initial position
  Serial.begin(9600);
}
 
void loop() {
  /*Variabelen om de som van de laatste 5 metingen en het aantal metingen op te slaan (5 metingen geeft het beste het verschil weer tussen reactiviteit en accuracy, hierdoor is de 
  overgang tussen metingen wat vloeiender en oogt het ook beter.)*/
  int som = 0;
  int teller = 0;
 
  // Neem 5 metingen en bereken hun gemiddelde
  for (int i = 0; i < 5; i++) {
    // Activeer de ultrasone sensor om een meting te starten
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
 
    // Meet de echo-tijd om de afstand te bepalen
    long duur = pulseIn(ECHO_PIN, HIGH);
 
    // Bereken de afstand tot het object (in cm)
    int afstand = duur * 0.034 / 2;
   
    // Voeg de afstand toe aan de som
    som += afstand;
   
    // metingen optellen
    teller++;
   
    // Als de afstand kleiner is dan of gelijk is aan 30 cm, pas de positie van de servo aan
    if (afstand <= 30) {
      // Bereken de positie van de servo lineair van 30cm (180 graden) naar 1cm (0 graden)
      // Door de afstand rechtstreeks te mappen naar de servo hoek
      int hoek = map(afstand, 1, 30, 0, 180); // Afstand van 1-30 cm wordt omgezet naar servo hoek van 0-180 graden
      servoMotor.write(hoek); // Positioneer de servo
    } else {
      // Als de afstand groter is dan 30 cm, beweeg de servo terug naar de initiële positie
      servoMotor.write(initialPosition); // Terug naar initiële positie
    }
  }
 
  // Bereken de gemiddelde afstand van de laatste drie metingen
  int gemiddelde_afstand = som / teller;
 
  // Geef de gemiddelde afstand weer op de seriële monitor
  Serial.print("Gemiddelde afstand tot object: ");
  Serial.print(gemiddelde_afstand);
  Serial.println(" cm");
 
  // Korte vertraging voor de volgende iteratie van de loop
  delay(100);
}
 
 
/*Uitleg code:
 
De omzetting van de gemeten echo-tijd naar centimeters gebeurt met behulp van de formule:
afstand=echo tijd×0.0342afstand=2echo tijd×0.034
Hierbij wordt aangenomen dat geluid zich met een snelheid van ongeveer 0,034 cm/μs door de lucht verplaatst. Omdat de ultrasone sensor de tijd meet die het 
geluid nodig heeft om heen en terug te reizen 
vanaf het object, wordt de gemeten tijd door twee gedeeld om de afstand tot het object te berekenen.
De aansturing van de servo naar een specifieke hoek wordt bepaald door de map() functie in de code. Deze functie werkt als volgt:
int hoek = map(afstand, 1, 30, 0, 180);
Dit betekent dat de variabele hoek wordt ingesteld op een waarde die wordt afgeleid van de variabele afstand. 
De map() functie herschaalt een waarde van het ene bereik naar het andere. In dit geval wordt de gemeten afstand (afstand) herschaald van het bereik van 1 tot 30 cm naar 
het bereik van 0 tot 180 graden (de mogelijke hoeken van de servo in onze bouw). Met andere woorden, de gemeten afstand wordt omgezet naar een geschikte hoek voor de servo,
 waarbij 1 cm overeenkomt met 0 graden en 30 cm overeenkomt met 180 graden. Deze lineaire mapping zorgt ervoor dat de servo de positie aanpast op basis van de gemeten afstand.
 heeft contextmenu
Opstellen.*/
