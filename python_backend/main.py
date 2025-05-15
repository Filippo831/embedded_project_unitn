import serial
import threading
import json
import subprocess

SERIAL_PORT = "/dev/ttyACM0"
# BAUD_RATE = 9600
BAUD_RATE = 9600


# Load the JSON file


def write_to_serial(ser, labels):
    ser.write(labels.encode("utf-8"))


def read_from_serial(ser, commands_dict):
    while True:
        if ser.in_waiting > 0:
            received = ser.readline().decode("utf-8", errors="ignore").strip()
            print(received)
            try:
                cmd = commands_dict[received]

                subprocess.run(cmd, shell=True)
            except:
                print(f"{received} not found")


def main():
    with open("configuration.json", "r") as file:
        data = json.load(file)

    labels = [item["label"] for item in data["commands"]]
    result_string = "\n".join(labels) + "!"

    # Create a dictionary from label: cmd
    commands_dict = {item["label"]: item["cmd"] for item in data["commands"]}
    try:
        ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)

        # Start writer thread
        writer_thread = threading.Thread(
            target=write_to_serial,
            args=(
                ser,
                result_string,
            ),
            daemon=True,
        )
        writer_thread.start()

        # Start reader loop in main thread
        read_from_serial(ser, commands_dict)

    except serial.SerialException as e:
        print(f"Serial error: {e}")
    except KeyboardInterrupt:
        print("Interrupted by user.")
    finally:
        if "ser" in locals() and ser.is_open:
            ser.close()
            print("Serial port closed.")


if __name__ == "__main__":
    main()
