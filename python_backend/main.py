import serial
import threading
import json
import subprocess

SERIAL_PORT = "/dev/ttyACM0"
BAUD_RATE = 9600


# write to serial communication
def write_to_serial(ser, labels):
    ser.write(labels.encode("utf-8"))


# read data from serial communication
def read_from_serial(ser, commands_dict):
    while True:
        if ser.in_waiting > 0:
            # receive value from serial
            received = ser.readline().decode("utf-8", errors="ignore").strip()

            # try to find the command attached to the label
            try:
                cmd = commands_dict[received]

                # run the command
                subprocess.run(cmd, shell=True)
            except:
                print(f"{received} not found")


def main():
    # load configuration and create a dict with the value read from the file
    with open("configuration.json", "r") as file:
        data = json.load(file)

    labels = [item["label"] for item in data["commands"]]
    result_string = "\n".join(labels) + "!"

    commands_dict = {item["label"]: item["cmd"] for item in data["commands"]}

    # init the serial communication
    try:
        ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)

        # start a thread with the read process
        writer_thread = threading.Thread(
            target=write_to_serial,
            args=(
                ser,
                result_string,
            ),
            daemon=True,
        )
        writer_thread.start()

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
