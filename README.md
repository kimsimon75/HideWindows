<a href="https://www.flaticon.com/kr/free-icons/-" title="오토 자이로 아이콘">오토 자이로 아이콘 제작자: Freepik - Flaticon</a>

오디오 PID 따는 코드 출처 : 아카라이브 neon_nyx 

본 프로그램은 마우스 사이드에 위치한 버튼을 누르게 되면 창 숨김과 보임, 음량 조절이 가능한 프로그램입니다.

꼭 관리자 권한으로 켜 주세요

마우스 XButton2 : 창 숨기기, 음소거

마우스 XButton1 : 창 보이기, 음소거 해제

F1 : 이제껏 숨긴 창 모두 보이기, 모두 음소거 해제

CTRL + ESC : 프로그램 종료

오디오의 경우 모든 오디오 세션과 그와 관련된 PID를 딴 후에 PID를 바탕으로 ProcessName 을 따고, map으로 ProcessName과 PID를 묶는 방식을 loop문으로 0.1초마다 갱신하는 식으로 오디오 세션을 잠그는 방식을 사용했습니다.

물론 큰 영향은 없겠지마는 map의 경우 추가는 되지만 삭제는 되지 않기에 본 프로그램이 돌아가는 동안 한 번이라도 타 프로그램의 오디오 세션이 켜진다면 꺼진 후에도 map에 그대로 남게 되며, 전과 같은 상황이 반복되게 되면 map에 processName과 PID가 쌓여 map 내의 find()함수의 탐색시간이 길어질 것으로 생각됩니다. 

단축키의 경우 후킹이라는 방식을 사용하여 원래의 기능은 잠구고 제가 원하는 기능을 수행하게 합니다. 문제는 후킹의 경우 다른 후킹과 충돌이 일어날 수 있고, 기존 기능보다 더 많은 기능을 수행해 cpu에 무리를 줄 수 있습니다.


또한 버그가 존재하긴 하는데 롤같은 경우 어째선지 숨기고 보이는 기능이 잘 작동되지 않아 이 점 참고 바랍니다.
