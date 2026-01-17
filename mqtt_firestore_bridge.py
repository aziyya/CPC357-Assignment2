import os
import paho.mqtt.client as mqtt
from google.cloud import firestore
import datetime

# --- CONFIGURATION ---
MQTT_BROKER_ADDRESS = "<MQTT_BROKER_ADDRESS>"
MQTT_TOPIC = "<MQTT_TOPIC>"
KEY_FILE = "service-key.json"
DATABASE_ID = "<DATABASE_ID>" # <--- PASTE YOUR DATABASE ID HERE

# --- 1. SETUP SECURITY (THE FIX) ---
# We force the system to use your Key File.
# This fixes the "403 ACCESS_TOKEN_SCOPE_INSUFFICIENT" error.
os.environ["GOOGLE_APPLICATION_CREDENTIALS"] = KEY_FILE

print(f"Trying to connect to Firestore Database: {DATABASE_ID}...")

try:
    # Connect using the Google Cloud library directly
    db = firestore.Client(database=DATABASE_ID)
    print("✅ Firestore Connected Successfully!")
except Exception as e:
    print(f"❌ Connection Error: {e}")
    exit()

# --- 2. DEFINE MQTT FUNCTIONS ---
def on_connect(client, userdata, flags, rc):
    print(f"✅ Connected to MQTT Broker! Listening to topic: {MQTT_TOPIC}")
    client.subscribe(MQTT_TOPIC)

def on_message(client, userdata, msg):
    try:
        payload = msg.payload.decode("utf-8")
        print(f"📩 Data Received: {payload}")

        # Prepare data
        doc_data = {
            u'timestamp': firestore.SERVER_TIMESTAMP,
            u'raw_data': payload,
            u'status': "FIRE DETECTED" if "1" in payload else "Normal"
        }

        # Upload to 'fire_logs' collection
        db.collection(u'fire_logs').add(doc_data)
        print("☁️  Saved to Cloud Database")

    except Exception as e:
        print(f"❌ Error uploading: {e}")

# --- 3. START LISTENING ---
client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

try:
    client.connect(MQTT_BROKER_ADDRESS, 1883, 60)
    client.loop_forever()
except KeyboardInterrupt:
    print("\nStopped.")