#include <WiFi.h>
#include <ESP_Mail_Client.h>

#define WIFI_SSID "Dane's Galaxy S23 Ultra"
#define WIFI_PASSWORD "aabbcc_1"
#define SMTP_server "smtp.gmail.com"
#define SMTP_Port 465
#define sender_email "microfire01@gmail.com"
#define sender_password "etcdxcpqkaprfrgb"
#define Recipient_email "carlogalacgac98@gmail.com"
#define Recipient_name ""

//13 = GPIO13 etc. 
int inputPinLocation1 = 13;
int inputPinLocation2 = 12;
int ouputPinRelay1 = 15;
int ouputPinRelay2 = 2;
String location1 = "Location 1";
String location2 = "Location 2";

SMTPSession smtp;

void setup() {
  Serial.begin(115200);
  pinMode(inputPinLocation1, INPUT);
  pinMode(inputPinLocation2, INPUT);
  pinMode(ouputPinRelay1, OUTPUT);
  pinMode(ouputPinRelay2, OUTPUT);
  Serial.println();
  Serial.println("Connecting...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(200);
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}

void triggerRelays(int value) {
  digitalWrite(ouputPinRelay1, value);
  digitalWrite(ouputPinRelay2, value);
}

void sendEmail(String loc) {
  smtp.debug(1);

  ESP_Mail_Session session;

  session.server.host_name = SMTP_server;
  session.server.port = SMTP_Port;
  session.login.email = sender_email;
  session.login.password = sender_password;
  session.login.user_domain = "";

  /* Declare the message class */

  SMTP_Message message;

  message.sender.name = "ESP 32";
  message.sender.email = sender_email;
  message.addRecipient(Recipient_name, Recipient_email);
  String htmlMsg;

  Serial.println("[Urgent] Fire has started at " + loc);
  message.subject = "[Urgent] Fire has started at " + loc;
  htmlMsg = "<div style=\"color:#880808;\"><h1> ALERT!</h1><p> Fire has started! </p></div>";

  message.html.content = htmlMsg.c_str();
  message.html.content = htmlMsg.c_str();
  message.text.charSet = "us-ascii";
  message.html.transfer_encoding = Content_Transfer_Encoding::enc_7bit;

  /* //Send simple text message

  String textMsg = "How are you doing";
  message.text.content = textMsg.c_str();
  message.text.charSet = "us-ascii";
  message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;*/

  if (!smtp.connect(&session)) {
    Serial.println("Error");
    return;
  }
  if (!MailClient.sendMail(&smtp, &message)) {
    Serial.println("Error sending Email, " + smtp.errorReason());
  }
}

void loop() {
  if (digitalRead(inputPinLocation1) == HIGH) {
    triggerRelays(1);
    Serial.println("loc1");
    sendEmail(location1);
    delay(15000);
    triggerRelays(0);
  } else if (digitalRead(inputPinLocation2) == HIGH) {
    triggerRelays(1);
    Serial.println("loc2");
    sendEmail(location2);
    delay1(5000);
    triggerRelays(0);
  }
}
