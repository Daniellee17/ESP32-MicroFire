#include <WiFi.h>
#include <ESP_Mail_Client.h>

#define WIFI_SSID "Meow"
#define WIFI_PASSWORD "Westbajacbajac01!"
#define SMTP_server "smtp.gmail.com"
#define SMTP_Port 465
#define sender_email "carlotest012@gmail.com"
#define sender_password "dkzlekuwgggjuuku"
#define Recipient_email "carlogalacgac98@gmail.com"
#define Recipient_name ""

int inputPinToSendEmail = 15;
int inputPinPillTaken = 14;
SMTPSession smtp;

void setup() {
  Serial.begin(115200);
  pinMode(inputPinToSendEmail, INPUT);
  pinMode(inputPinPillTaken, INPUT);
  Serial.println();
  Serial.print("Connecting...");
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

void sendEmail() {
  smtp.debug(1);
  ESP_Mail_Session session;

  session.server.host_name = SMTP_server;
  session.server.port = SMTP_Port;
  session.login.email = sender_email;
  session.login.password = sender_password;
  session.login.user_domain = "";

  SMTP_Message message;
  message.sender.name = "ESP 32";
  message.sender.email = sender_email;
  message.addRecipient(Recipient_name, Recipient_email);
  String htmlMsg;

  if (digitalRead(inputPinPillTaken) == HIGH) {
    Serial.println("Pill has been taken...");
    message.subject = "[Urgent] Pill has been taken";
    htmlMsg = "<div style=\"color:#000000;\"><h1> ALERT!</h1><p> Mail Generated from ESP32</p><p> Pill has been taken </p></div>";
  } else {
    Serial.println("Pill has not been taken...");
    message.subject = "[Urgent] Pill has not been taken";
    htmlMsg = "<div style=\"color:#000000;\"><h1> ALERT!</h1><p> Mail Generated from ESP32</p><p> Pill has not been taken </p></div>";
  }

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
  if (digitalRead(inputPinToSendEmail) == HIGH) {
    Serial.println("Sending email...");
    if (digitalRead(inputPinPillTaken) == HIGH) {
      Serial.println("Pill has been taken...");
    } else {
      Serial.println("Pill has not been taken...");
    }
    sendEmail();
    delay(4000);
  } else {
    Serial.println("Waiting for signal...");
  }
}