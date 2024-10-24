import asyncio
from bleak import BleakClient

# 장치 주소는 스캔한 결과에서 확인한 실제 BLE 장치 주소로 변경하세요.
BLE_ADDRESS = "C4:D3:6A:9A:5C:08"  # BLE 모듈의 실제 주소를 입력

# BLE UUIDs
SERVICE_UUID = "0000fff0-0000-1000-8000-00805f9b34fb"
CHARACTERISTIC_UUID_RX = "0000fff4-0000-1000-8000-00805f9b34fb"  # Read Characteristic
CHARACTERISTIC_UUID_TX = "0000fff1-0000-1000-8000-00805f9b34fb"  # Write Characteristic

async def run():
    async with BleakClient(BLE_ADDRESS) as client:
        # BLE 장치와 연결 성공 여부 확인
        if await client.is_connected():
            print(f"Connected to BLE device with address: {BLE_ADDRESS}")

            # RX 특성에서 데이터 읽기
            rx_data = await client.read_gatt_char(CHARACTERISTIC_UUID_RX)
            print(f"Received data from RX characteristic: {rx_data.decode('utf-8')}")

            # TX 특성으로 데이터 전송
            tx_data = "Hello BLE"
            await client.write_gatt_char(CHARACTERISTIC_UUID_TX, tx_data.encode('utf-8'))
            print(f"Sent data to TX characteristic: {tx_data}")
        else:
            print("Failed to connect to the BLE device")

# 비동기 함수 실행
loop = asyncio.get_event_loop()
loop.run_until_complete(run())
