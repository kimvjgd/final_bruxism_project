import asyncio
from bleak import BleakClient, BleakScanner

# BLE 장치의 UUID
SERVICE_UUID = "0000fff0-0000-1000-8000-00805f9b34fb"
CHARACTERISTIC_UUID_RX = "0000fff4-0000-1000-8000-00805f9b34fb"  # 읽기 또는 Notify용 특성
CHARACTERISTIC_UUID_TX = "0000fff1-0000-1000-8000-00805f9b34fb"  # 쓰기 특성

# BLE 장치 주소 (스캔 결과에서 얻은 실제 주소로 변경하세요)
BLE_ADDRESS = "C4:D3:6A:9A:5C:08"

# Notify를 수신하는 핸들러
def notification_handler(sender, data):
    print(f"Notification from {sender}: {data.decode('utf-8')}")

async def run():
    async with BleakClient(BLE_ADDRESS) as client:
        if await client.is_connected():
            print(f"Connected to BLE device with address: {BLE_ADDRESS}")

            try:
                # 특성에서 데이터를 읽으려고 시도
                print("Attempting to read from the characteristic...")
                rx_data = await client.read_gatt_char(CHARACTERISTIC_UUID_RX)
                print(f"Received data from RX characteristic: {rx_data.decode('utf-8')}")
            except Exception as e:
                # 읽기 실패 시 Notify 기능 사용 시도
                print(f"Read failed: {e}. Trying Notify...")

                # Notify를 활성화하고 데이터 수신
                await client.start_notify(CHARACTERISTIC_UUID_RX, notification_handler)

                # 무기한 대기
                print("Waiting for notifications indefinitely. Press Ctrl+C to stop.")
                while True:
                    await asyncio.sleep(1)  # 1초마다 대기하며 Notify 계속 수신

                # Notify 비활성화 (이 코드는 무기한 대기 상태에선 실행되지 않음)
                await client.stop_notify(CHARACTERISTIC_UUID_RX)
                print("Stopped receiving notifications.")

            # TX 특성으로 데이터 전송
            # tx_data = "Hello BLE"
            # await client.write_gatt_char(CHARACTERISTIC_UUID_TX, tx_data.encode('utf-8'))
            # print(f"Sent data to TX characteristic: {tx_data}")
        else:
            print("Failed to connect to the BLE device")


# 비동기 함수 실행
loop = asyncio.get_event_loop()
loop.run_until_complete(run())
