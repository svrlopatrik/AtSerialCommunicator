void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);

  delay(1000);
  Serial.println("AtSerialCommunicator is ready!");
  Serial.println();
}

char* readDataFromSerial();    
char* removeNlCr(char*, int);
char* addNlCr(char*, int);
char* sendAtCommand(char*);  

void loop() {

  char* serialInput = readDataFromSerial();
   if(serialInput) {
      Serial.print("Command: ");
      Serial.println(serialInput);
      char* atResponse = sendAtCommand(serialInput);
      if(atResponse) {
        Serial.println(atResponse);
        free(atResponse);
      } else {
        Serial.println("No response");
        Serial.println();
      }
      
      free(serialInput);
   }
}

char* readDataFromSerial() {
  int availableSize = Serial.available();
  if(availableSize) {
    char* data = (char*) calloc(availableSize, sizeof(char));
    if(data) {
      Serial.readBytes(data, availableSize);
      return removeNlCr(data, availableSize);
    } else {
      Serial.println("readDataFromSerial() -> cannot allocate space");
      return NULL;
    }
  }

  return NULL;
}

char* removeNlCr(char* data, int size) {
  if(size >= 2) {
        if(data[size - 2] == '\r' && data[size - 1] == '\n') {
          data = (char*) realloc(data, (size - 1) * sizeof(char));
          data[size - 2] = '\0';
        } else if(data[size - 1] == '\n' || data[size - 1] == '\r') {
          data[size - 1] = '\0';
        } else {
          data = (char*) realloc(data, (size + 1) * sizeof(char));
          data[size] = '\0';
        }
   } else {
     if(data[size - 1] == '\n' || data[size - 1] == '\r') {
       data[size - 1] = '\0';
     } else {
       data = (char*) realloc(data, (size + 1) * sizeof(char));
       data[size] = '\0';
     }
  }

  return data;
}

char* sendAtCommand(char* command) {
  Serial1.write(addNlCr(command, strlen(command)), strlen(command) + 2);  // +2 - NL and CR
  delay(100);

  int availableSize = Serial1.available();
  if(availableSize) {
    char* response = (char*) calloc(availableSize + 1, sizeof(char));
    if(response) {
      Serial1.readBytes(response, availableSize);
      response[availableSize] = '\0';
      return response;
    } else {
      Serial.println("sendAtCommand(char*) -> cannot allocate space");
      return NULL;
    }
  }

  return NULL;
}

char* addNlCr(char* data, int size) {
  data = (char*) realloc(data, (size + 2) * sizeof(char));
  
  data[size]      = '\r';
  data[size + 1]  = '\n';

  return data;
}


