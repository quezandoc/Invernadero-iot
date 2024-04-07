
/**
 * Created by K. Suwatchai (Mobizt)
 *
 * Email: k_suwatchai@hotmail.com
 *
 * Github: https://github.com/mobizt/Firebase-ESP-Client
 *
 * Copyright (c) 2023 mobizt
 *
 */

// This example shows how to create a document in a document collection. This operation required Email/password, custom or OAUth2.0 authentication.

#include <Arduino.h>
#if defined(ESP32) || defined(ARDUINO_RASPBERRY_PI_PICO_W)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#elif __has_include(<WiFiNINA.h>)
#include <WiFiNINA.h>
#elif __has_include(<WiFi101.h>)
#include <WiFi101.h>
#elif __has_include(<WiFiS3.h>)
#include <WiFiS3.h>
#endif

#include <Firebase_ESP_Client.h>

#include <DFRobot_DHT11.h>
DFRobot_DHT11 DHT;
#define DHT11_PIN 2
#define LED 12 //Seleccionamos el pin en el que se //conectará el led

// Provide the token generation process info.
#include <addons/TokenHelper.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "WIFI_NAME"
#define WIFI_PASSWORD "WIFI_PASS"

/* 2. Define the API Key */
#define API_KEY "AIzaSyB-nP0BV0qwgK4tE_dMi2W2cihn8Z7rldQ"

/* 3. Define the project ID */
#define FIREBASE_PROJECT_ID "proyectoinvernadero-14daf"

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "admin@admin.com"
#define USER_PASSWORD "admin123"

// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long dataMillis = 0;
int count = 0;

#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
WiFiMulti multi;
#endif

// The Firestore payload upload callback function
void fcsUploadCallback(CFS_UploadStatusInfo info)
{
    if (info.status == firebase_cfs_upload_status_init)
    {
        Serial.printf("\nUploading data (%d)...\n", info.size);
    }
    else if (info.status == firebase_cfs_upload_status_upload)
    {
        Serial.printf("Uploaded %d%s\n", (int)info.progress, "%");
    }
    else if (info.status == firebase_cfs_upload_status_complete)
    {
        Serial.println("Upload completed ");
    }
    else if (info.status == firebase_cfs_upload_status_process_response)
    {
        Serial.print("Processing the response... ");
    }
    else if (info.status == firebase_cfs_upload_status_error)
    {
        Serial.printf("Upload failed, %s\n", info.errorMsg.c_str());
    }
}

void setup()
{

    Serial.begin(115200);
    pinMode(LED, OUTPUT);  // Define el pin 12 como salida
#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
    multi.addAP(WIFI_SSID, WIFI_PASSWORD);
    multi.run();
#else
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
#endif

    Serial.print("Connecting to Wi-Fi");
    unsigned long ms = millis();
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(300);
#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
        if (millis() - ms > 10000)
            break;
#endif
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

    /* Assign the api key (required) */
    config.api_key = API_KEY;

    /* Assign the user sign in credentials */
    auth.user.email = USER_EMAIL;
    auth.user.password = USER_PASSWORD;

    // The WiFi credentials are required for Pico W
    // due to it does not have reconnect feature.
#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
    config.wifi.clearAP();
    config.wifi.addAP(WIFI_SSID, WIFI_PASSWORD);
#endif

    /* Assign the callback function for the long running token generation task */
    config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

    // Comment or pass false value when WiFi reconnection will control by your code or third party library e.g. WiFiManager
    Firebase.reconnectNetwork(true);

    // Since v4.4.x, BearSSL engine was used, the SSL buffer need to be set.
    // Large data transmission may require larger RX buffer, otherwise connection issue or data read time out can be occurred.
    fbdo.setBSSLBufferSize(4096 /* Rx buffer size in bytes from 512 - 16384 */, 1024 /* Tx buffer size in bytes from 512 - 16384 */);

    // Limit the size of response payload to be collected in FirebaseData
    fbdo.setResponseSize(2048);

    Firebase.begin(&config, &auth);

    // For sending payload callback
    // config.cfs.upload_callback = fcsUploadCallback;

    // You can use TCP KeepAlive in FirebaseData object and tracking the server connection status, please read this for detail.
    // https://github.com/mobizt/Firebase-ESP-Client#about-firebasedata-object
    // fbdo.keepAlive(5, 5, 1);
}

void loop()
{

    // Firebase.ready() should be called repeatedly to handle authentication tasks.

    if (Firebase.ready() && (millis() - dataMillis > 60000 || dataMillis == 0))
    {
        dataMillis = millis();

        // For the usage of FirebaseJson, see examples/FirebaseJson/BasicUsage/Create_Edit_Parse/Create_Edit_Parse.ino
        FirebaseJson content;

        // Note: If new document created under non-existent ancestor documents, that document will not appear in queries and snapshot
        // https://cloud.google.com/firestore/docs/using-console#non-existent_ancestor_documents.

        // We will create the document in the parent path "a0/b?
        // a0 is the collection id, b? is the document id in collection a0.

        String documentPath = "Lechuga";

        // If the document path contains space e.g. "a b c/d e f"
        // It should encode the space as %20 then the path will be "a%20b%20c/d%20e%20f"

        DHT.read(DHT11_PIN);
        float h = DHT.humidity; //Se lee la humedad
        float t = DHT.temperature; //Se lee la temperatura
        //Se imprimen las variables
        Serial.println("Humedad: ");
        Serial.println(h);
        Serial.println("Temperatura: ");
        Serial.println(t);

        // Caso de tomates
        if ((t < 16 || t > 27) || (h < 60 || h > 70)) {
            Serial.print("Alarma--------------- ");
            digitalWrite(LED, HIGH); // Enciende LED para alarma
        } else {
            digitalWrite(LED, LOW); // Apaga LED si no hay alarma
        }

        // Caso de lechugas
        if ((t < 10 || t > 24) || (h < 50 || h > 70)) {
            Serial.print("Alarma--------------- ");
            digitalWrite(LED, HIGH); // Enciende LED para alarma
        } else {
            digitalWrite(LED, LOW); // Apaga LED si no hay alarma
        }

        // Caso de zapallo
        if ((t < 18 || t > 32) || (h < 60 || h > 80)) {
            Serial.print("Alarma--------------- ");
            digitalWrite(LED, HIGH); // Enciende LED para alarma
        } else {
            digitalWrite(LED, LOW); // Apaga LED si no hay alarma
        }

        // Caso de papas
        if ((t < 10 || t > 22) || (h < 80 || h > 90)) {
            Serial.print("Alarma--------------- ");
            digitalWrite(LED, HIGH); // Enciende LED para alarma
        } else {
            digitalWrite(LED, LOW); // Apaga LED si no hay alarma
        }
        
        // integer
        content.set("fields/alive/doubleValue", millis()/60000);
        content.set("fields/temperature/integerValue", String(DHT.temperature));
        content.set("fields/humidity/integerValue", String(DHT.humidity));

        String doc_path = "projects/";
        doc_path += FIREBASE_PROJECT_ID;
        doc_path += "/databases/(default)/documents/coll_id/doc_id"; // coll_id and doc_id are your collection id and document id

        Serial.print("Create a document... ");

        if (Firebase.Firestore.createDocument(&fbdo, FIREBASE_PROJECT_ID, "" /* databaseId can be (default) or empty */, documentPath.c_str(), content.raw()))
            Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
        else
            Serial.println(fbdo.errorReason());
    }
}
