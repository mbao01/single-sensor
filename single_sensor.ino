const int len = 4;

int current_sensor_8_bit[len],
    current_sensor_amp[len],
    HOLD_current_sensor_amp[len],
    maxValue[len],
    minValue[len];

const int SENSOR1 = 0,
          SENSOR2 = 1,
          AC712_SENSOR_OFFSET = 512,
          MAX_DIGITAL_VALUE = 1023;

const double AC712_SENSOR_VOLT_AMP_CONVERSION = 27.03;

const int SWITCH1 = 4,
          SWITCH2 = 5;
      
void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  
  initializeSensor(SENSOR1);
  initializeSensor(SENSOR2);
  initializeOutput(SWITCH1);
  initializeOutput(SWITCH2);
  
  delay(500);

  Serial.println("< < < System initialize completely ");
  Serial.println("> > >");
  Serial.println("");
  
}


void loop() {
  // put your main code here, to run repeatedly:

  if(readValue(SENSOR1) <= 1.56) {
    Switch(SWITCH1, 1);
    Switch(SWITCH2, 0);
  }
  
  readValue(SENSOR2);

  delay(500);
  
}

int readValue(int pos) {
  
  current_sensor_8_bit[pos] = analogRead(pos);
  current_sensor_amp[pos] = (((current_sensor_8_bit[pos] - AC712_SENSOR_OFFSET) * AC712_SENSOR_VOLT_AMP_CONVERSION ) / MAX_DIGITAL_VALUE);
  current_sensor_amp[pos] = (current_sensor_amp[pos] + HOLD_current_sensor_amp[pos]) / 2;
  HOLD_current_sensor_amp[pos] = current_sensor_amp[pos];
  
  maxValue[pos] = max(current_sensor_amp[pos], maxValue[pos]);  minValue[pos] = min(current_sensor_amp[pos], minValue[pos]);

  Serial.println(current_sensor_amp[pos] + "  " + maxValue[pos] - minValue[pos]);

  delay(50);

  return current_sensor_amp[pos];
  
}

void initializeSensor(int pos) {
  
  pinMode(pos, INPUT);
  current_sensor_8_bit[pos] = analogRead(pos);
  current_sensor_amp[pos] = (((current_sensor_8_bit[pos] - AC712_SENSOR_OFFSET) * AC712_SENSOR_VOLT_AMP_CONVERSION ) / MAX_DIGITAL_VALUE);
  maxValue[pos] = minValue[pos] = HOLD_current_sensor_amp[pos] = current_sensor_amp[pos];

  delay(25);
  
}

void initializeOutput(int pos) {
  
  pinMode(pos, OUTPUT);
  
  delay(25);
  
}

void Switch(int pos, int state) {
  if (state)
    digitalWrite(pos, HIGH);
  else
    digitalWrite(pos, LOW);
  
}

