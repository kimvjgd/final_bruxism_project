import time

def print_incrementing_seconds():
    seconds = 0  # 시작 시간을 0초로 설정
    try:
        while True:
            print(f"현재 시간: {seconds} 초")
            seconds += 1  # 1초 증가
            time.sleep(1)  # 1초 대기
    except KeyboardInterrupt:
        print("프로그램을 종료합니다.")

if __name__ == "__main__":
    print_incrementing_seconds()
