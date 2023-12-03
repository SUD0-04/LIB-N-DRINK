#include <stdio.h>
#include <string.h>

// usleep 함수 사용을 위해 추가
#include <unistd.h>

// 상수 정의
#define MAX_USERS 20
#define MAX_NAME_LENGTH 50
#define MAX_STUDENT_ID_LENGTH 10
#define MAX_PAYMENT_METHOD_LENGTH 10
#define NUM_SEATS 100
#define MAX_PASS_LENGTH 12

// 사용자 정보 구조체
typedef struct {
    int id; 
    char name[MAX_NAME_LENGTH];
    long long studentID[MAX_STUDENT_ID_LENGTH];
    int grade;
    char department[MAX_NAME_LENGTH];
    char paymentMethod[MAX_PAYMENT_METHOD_LENGTH];
    char password[MAX_PASS_LENGTH]; 
    int balance;
    int isLoggedIn;
} User;

// 사용자 정보 구조체의 배열을 동적할당으로 변경
User *users = NULL;

void manageUserInfo(User *user);
void registerPaymentMethod(User *user);
void showUserInfo(User *user);
void modifyUserInfo(User *user);
void showSeatMap(int seats[], int num_seats);

// 음료 주문 기능 함수들
typedef struct {
    char name [MAX_NAME_LENGTH];
    int price;
} Drink;

typedef struct {
    int order_id;
    char drink_name[MAX_NAME_LENGTH];
    int price;
} Order;


void loading_bar(int delay) {
    int progress;

    printf("\n[로딩바]\n");
    for (progress = 0; progress <= 100; progress++) {
        printf("\r[");
        for (int i = 0; i < progress; i += 5) {
            printf("#");
        }
        for (int i = progress + 5; i <= 100; i += 5) {
            printf(" ");
        }
        printf("] %d%%", progress);
        fflush(stdout);
        // 입력된 'delay' 값에 따른 딜레이
        usleep(delay * 1000);
    }
    printf("\n");
}

// 자리 예약 기능 함수들
// 자리 현황을 출력하는 함수
void showSeatMap(int seats[], int num_seats) {
    int row = 10;
    int col = 10;
    printf("[자리 예약 및 취소]\n");
    printf("[자리 현황]\n");
    for (int r = 0; r < row; r++) {
        for (int c = 0; c < col; c++) {
            int seat_number = r * col + c;
            if (seats[seat_number]) {
                printf("[ X ] ");
            } else {
                printf("[ %d ] ", seat_number + 1);
            }
        }
        printf("\n");
    }
}



// 자리 예약 및 예약 취소를 담당하는 함수
void reserveSeat(int seats[], int numSeats, int reserve) {
    int seatNumber;
    while (1) {
        if (reserve) {
            printf("예약할 자리 번호를 입력하세요 (뒤로가기: 99): ");
        } else {
            printf("취소할 자리 번호를 입력하세요 (뒤로가기: 99): ");
        }
        scanf("%d", &seatNumber);

        if (seatNumber == 99)
            return;

        if (seatNumber < 1 || seatNumber > numSeats) {
            printf("유효하지 않은 자리 번호입니다.\n");
            continue;
        }
        break;
    }

    // 자리 예약을 처리하는 분기
    if (reserve) {
        if (seats[seatNumber - 1] == 1) {
            printf("이미 예약된 자리입니다. 다른 자리를 선택하세요.\n");
        } else {
            seats[seatNumber - 1] = 1;
            printf("자리 예약이 완료되었습니다. 자리 번호: %d\n", seatNumber);
        }
    } else { // 자리 예약 취소 처리하는 분기
        if (seats[seatNumber - 1] == 0) {
            printf("이미 예약 가능한 자리입니다. 다른 자리를 선택하세요.\n");
        } else {
            seats[seatNumber - 1] = 0;
            printf("자리 예약이 취소되었습니다. 자리 번호: %d\n", seatNumber);
        }
    }
}

// 회원 가입 기능 함수
int signUp(User users[], int numUsers) {
    if (numUsers >= MAX_USERS) {
        printf("회원 가입이 불가능합니다. 최대 회원 수를 초과하였습니다.\n");
        return -1;
    }

    User newUser;

    printf("[회원 가입]\n");
    printf("이름: ");
    scanf("%s", newUser.name);
    printf("학번: ");
    scanf("%lld", newUser.studentID);
    printf("학년: ");
    scanf("%d", &newUser.grade);
    printf("학과 또는 학부: ");
    scanf("%s", newUser.department);

    // 결제 수단 등록 선택을 처리하는 부분
    char registerPaymentChoice;
    printf("결제 수단을 등록하시겠습니까? (Y/N): ");
    scanf(" %c", &registerPaymentChoice);

    if (registerPaymentChoice == 'Y' || registerPaymentChoice == 'y') {
        printf("1. 카드\n");
        printf("2. 현금\n");
        printf("결제 수단을 선택하세요: ");
        int paymentChoice;
        scanf("%d", &paymentChoice);

        if (paymentChoice == 1) {
            strcpy(newUser.paymentMethod, "카드");
        } else if (paymentChoice == 2) {
            strcpy(newUser.paymentMethod, "현금");
        } else {
            printf("잘못된 입력입니다. 결제 수단은 등록되지 않았습니다.\n");
        }
    }
    
    printf("비밀번호: ");
    scanf("%s", newUser.password);

    // 회원 정보 추가
    users[numUsers] = newUser;
    printf("회원 가입이 완료되었습니다.\n");

    return numUsers + 1;
}

// 로그인 기능 함수
int login(User users[], int numUsers) {
    char studentID[MAX_STUDENT_ID_LENGTH];
    char password[MAX_PASS_LENGTH];
    printf("학번을 입력하세요: ");
    scanf("%lld", studentID);
    printf("비밀번호를 입력하세요: ");
    scanf("%s", password);

    for (int i = 0; i < numUsers; i++) {
        if (strcmp(users[i].studentID, studentID) == 0 && strcmp(users[i].password, password) == 0) {
            printf("환영합니다, %s님!\n", users[i].name);
            users[i].isLoggedIn = 1; // 로그인 상태로 변경
            return i;
        }
    }
    printf("일치하는 회원 정보가 없습니다. 다시 로그인해주세요.\n"); // 로그인 실패 시 메시지 추가
    return -1;
}


const Drink drinks[] = {
    {"아메리카노", 1500},
    {"카페 라떼", 2000},
    {"일반 요거트", 2500},
    {"딸기 요거트", 2500},
    {"녹차 푸라푸치노", 3000},
    {"자바초코칩 푸라푸치노", 3000},
    {"오레오 푸라푸치노", 3000},
    {"유자차", 2500},
    {"아이스티", 2500}
};

// 음료 메뉴 출력하는 함수
void showDrinkMenu() {
    printf("[음료 주문]\n");

    printf("음료 메뉴:\n");
    for (int i = 0; i < sizeof(drinks) / sizeof(drinks[0]); i++) {
        printf("%d. %s - %d원\n", i + 1, drinks[i].name, drinks[i].price);
    }
}

void orderDrink(User users[], int currentUser, Order orderHistory[], int *numOrders) {
    // 음료 선택 및 결제를 처리하는 함수
    int drinkChoice;
    while (1) {
        printf("주문할 음료 번호를 입력하세요 (뒤로가기: 99): ");
        scanf("%d", &drinkChoice);

        if (drinkChoice == 99)
            return;

        if (drinkChoice < 1 || drinkChoice > sizeof(drinks) / sizeof(drinks[0])) {
            printf("유효하지 않은 음료 번호입니다.\n");
            continue;
        }
        break;
    }

    printf("결제 수단을 선택하세요 (카드 또는 현금): ");
    char paymentMethod[MAX_PAYMENT_METHOD_LENGTH];
    scanf("%s", paymentMethod);

    printf("음료 주문이 완료되었습니다. 주문번호: %d\n", drinkChoice);
}

// 기록 조회 기능 함수들
// 자리 예약 기록을 조회하는 함수
void showReservationRecord(int seats[], int numSeats) {
    printf("[자리 예약 기록 조회]\n");

    printf("현재 자리 상태:\n");
    for (int i = 0; i < numSeats; i++) {
        printf("[%d] ", i + 1);
        if (seats[i] == 1) {
            printf("예약됨\n");
        } else {
            printf("예약 가능\n");
        }
    }
}

// 음료 주문 기록 조회 함수 (구현 미완성)
void showOrderRecord(User users[], int currentUser, Order orderHistory[], int numOrders) {
    printf("[음료 주문 기록 조회]\n");
    int numUserOrders = 0;

    for (int i = 0; i < numOrders; i) {
        if (orderHistory[i].order_id == users[currentUser].id) {
            printf("%d. %s - %d원\n", numUserOrders + 1, orderHistory[i].drink_name, orderHistory[i].price);
            numUserOrders++;
        }
    }

    if (numUserOrders == 0) {
        printf("주문 내역이 없습니다.\n");
    }
}

// 설정 기능 함수들
void settings(User* user) {
    printf("[설정]\n");
    printf("1. 회원 정보 관리\n");
    printf("2. 결제 수단 등록\n");
    printf("메뉴를 선택하세요: ");
    int choice;
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            manageUserInfo(user);
            break;
        case 2:
            registerPaymentMethod(user);
            break;
        default:
            printf("잘못된 입력입니다. 메인 메뉴로 돌아갑니다.\n");
    }
}

// 회원 정보 조회 및 회원 정보 수정을 처리하는 함수
void manageUserInfo(User* user) {
    int choice;
    while (1) {
        printf("[회원 정보 관리]\n");
        printf("1. 회원 정보 조회\n");
        printf("2. 회원 정보 수정\n");
        printf("3. 뒤로가기\n");
        printf("메뉴를 선택하세요: ");
        scanf("%d", &choice);

        if (choice == 3)
            return;

        switch (choice) {
            case 1:
                showUserInfo(user);
                break;
            case 2:
                modifyUserInfo(user);
                break;
            default:
                printf("잘못된 입력입니다. 다시 입력해주세요.\n");
        }
    }
} 

// 회원 정보 조회 함수
void showUserInfo(User* user) {
    printf("[회원 정보 조회]\n");
    printf("이름: %s\n", user->name);
    printf("학번: %lld\n", user->studentID);
    printf("학년: %d\n", user->grade);
    printf("학과 또는 학부: %s\n", user->department);
    printf("결제 수단: %s\n", user->paymentMethod);
    printf("잔액: %d원\n", user->balance);
}

// 회원 정보 수정 함수
void modifyUserInfo(User* user) {
    printf("[회원 정보 수정]\n");
    printf("전체 정보를 수정하시겠습니까? (Y/N): ");
    char choice;
    scanf(" %c", &choice);

    if (choice == 'Y' || choice == 'y') {
        printf("이름: ");
        scanf("%s", user->name);
        printf("학번: ");
        scanf("%lld", user->studentID);
        printf("학년: ");
        scanf("%d", &user->grade);
        printf("학과 또는 학부: ");
        scanf("%s", user->department);
    }

    printf("메인 메뉴로 돌아갑니다.\n");
}

// 결제 수단 등록 함수
void registerPaymentMethod(User* user) {
    printf("[결제 수단 등록]\n");
    printf("1. 카드\n");
    printf("2. 현금\n");
    printf("결제 수단을 선택하세요: ");
    int choice;
    scanf("%d", &choice);

    if (choice == 1) {
        printf("카드 번호를 입력하세요 (-로 구분하여 16자리): ");
        char cardNumber[20];
        scanf("%s", cardNumber);
        printf("CVC를 입력하세요: ");
        char cvc[4];
        scanf("%s", cvc);

        strcpy(user->paymentMethod, "카드");
        printf("카드가 등록되었습니다.\n");
    } else if (choice == 2) {
        printf("현금을 입금할 금액을 입력하세요: ");
        int cash;
        scanf("%d", &cash);
        user->balance += cash;

        strcpy(user->paymentMethod, "현금");
        printf("현금이 등록되었습니다. 잔액: %d원\n", user->balance);
    } else {
        printf("잘못된 입력입니다. 메인 메뉴로 돌아갑니다.\n");
    }
}

// 관리자용 예약 정보 표시 함수
void display_reservations_status(int seats[], int numSeats, User users[], int numUsers) {
    printf("\n[예약 현황]\n");
    for (int i = 0; i < numSeats; i++) {
        if (seats[i] != -1) {
            printf("자리번호: %d 사용자ID: %d 이름: %s\n", i, users[seats[i]].id, users[seats[i]].name);
        } else {
            printf("자리번호: %d 예약가능\n", i);
        }
    }
}

// 관리자용 예약 관리 함수
void manage_reservations(int seats[], int numSeats, User users[], int numUsers) {
    int adminMenuChoice;
    do {
        printf("\n[관리자 메뉴]\n");
        printf("1. 예약 현황 표시\n");
        printf("2. 예약 변경\n");
        printf("3. 예약 삭제\n");
        printf("4. 돌아가기\n");
        printf("메뉴 선택: ");
        scanf("%d", &adminMenuChoice);

        switch (adminMenuChoice) {
            // 예약 현황 표시
            case 1:
                display_reservations_status(seats, numSeats, users, numUsers);
                break;
            // 예약 변경 기능 구현 (예정)
            case 2:
                break;
            // 예약 삭제 기능 구현 (예정)
            case 3:
                break;
            // 돌아가기
            case 4:
                break;
            default:
                printf("잘못된 입력입니다. 다시 입력해주세요.\n");
        }
    } while (adminMenuChoice != 4);
}

// 메인 함수
int main(void) {
    char adminPassword[20];
    User users[MAX_USERS];
    int numUsers = 0;
    int currentUser = -1;
    int seats[NUM_SEATS] = {0};
    Order orderHistory[MAX_USERS * 40]; // 각 사용자별로 최대 40개의 주문 내역을 가정
    int Orders = 0;
    int numOrders = 0;


    while(1){
        while(currentUser == -1){
            int menuChoice;
            printf("\n1. 로그인\n");
            printf("2. 회원 가입\n");
            printf("0. 종료\n");
            printf("원하시는 기능의 번호를 입력하세요: ");
            scanf("%d", &menuChoice);

            switch (menuChoice) {
                case 1:
                    loading_bar(5);
                    currentUser = login(users, numUsers);
                    break;
                case 2:
                    loading_bar(5);
                    numUsers = signUp(users, numUsers);
                    break;
                case 0:
                    printf("프로그램을 종료합니다.\n");
                    return 0;
                default:
                    printf("알 수 없는 입력입니다. 다시 시도해주세요.\n");
            }
        }

    while (currentUser != -1) {
        printf("\n[메인 메뉴]\n");
        printf("1. 자리 예약\n");
        printf("2. 음료 주문\n");
        printf("3. 기록 조회\n");
        printf("4. 설정\n");
        printf("5. 관리자 모드\n");
        printf("0. 종료\n");
        printf("메뉴를 선택하세요: ");
        int menuChoice;
        scanf("%d", &menuChoice);

        // 메인 메뉴 switch문으로 각각 기능 처리
        switch (menuChoice) {
            case 0:
                printf("프로그램을 종료합니다.\n");
                return 0;
            case 1:
                // 예약 전 로딩바를 보여줌
                loading_bar(10);
                if (currentUser >= 0) { // 로그인한 사용자가 있을 때만 자리 예약 기능을 사용하도록 추가
                    showSeatMap(seats, NUM_SEATS);
                    printf("1. 자리 예약\n");
                    printf("2. 예약 취소\n");
                    printf("옵션을 선택하세요: ");

                    int reserve;
                    scanf("%d", &reserve);
            
                if (reserve == 1) {
                    reserveSeat(seats, NUM_SEATS, 1);
                    } else if (reserve == 2) {
                        reserveSeat(seats, NUM_SEATS, 0);
                    } else {
                        printf("잘못된 입력입니다.\n");
                    }
                    } else {
                        printf("자리 예약을 하려면 먼저 로그인해야 합니다.\n");
                     }
                break;
            // 음료 주문
            case 2:
                showDrinkMenu();
                orderDrink(users, currentUser, orderHistory, &numOrders);
                break;
            // 자리 예약
            case 3:
                showReservationRecord(seats, NUM_SEATS);
                showOrderRecord(users, currentUser, orderHistory, numOrders);
                break;
            // 설정
            case 4:
                settings(&users[currentUser]);
                break;
            // 관리자용 기능 추가
            case 5:
                // 비밀번호 검증 코드 추가
                printf("관리자 비밀번호를 입력하세요: ");
                scanf("%s", adminPassword); 

                // 비밀번호가 일치하는지 확인
                // 임시 비밀번호로 1234를 사용
                if (strcmp(adminPassword, "1234") == 0) {
                    // 로그인한 사용자를 관리자로 가정하고 기능을 실행
                    if (currentUser == 0) { 
                        manage_reservations(seats, NUM_SEATS, users, numUsers);
                    } else {
                        printf("관리자 전용 기능입니다.\n");
                    }
                } else {
                    printf("비밀번호가 일치하지 않습니다.\n");
                }
                break;
            default:
                printf("잘못된 입력입니다. 메뉴를 다시 선택해주세요.\n");
        }
    }
        
    }

    return 0;
}  