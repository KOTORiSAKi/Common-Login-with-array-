#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <omp.h>
#include <ctype.h>

//*Release updated 10-10-24
//*V 1.0.3.26 updated (Final Project Release Version)
//TODO=> improved performance , Some UI was fixed

//?completed codes : 99.97% | bug : 0.9% | logic error : 0.55% (Just guessing)

//!setting
#define MAX_USER 100 //to access maximum user accounts in slots
#define MAX_STRING 20 //to config how many character can store in string
//--------------------------
char username[MAX_STRING+1];
char phone[MAX_STRING+1];
char password[MAX_STRING+1];
int login[3] = {0};//to tell that program still be logged in or not
char *log_username[MAX_USER+2];
char *log_phone[MAX_USER+2];
char *log_password[MAX_USER+2];
//--------------------------
int current_user = 0;
int checker1=0, checker2=0, checker3=0;
int new_user = 0;

void exit_point(){
    loadingBar(2);
    printf("\nExited\n");
    exit(0);
}

void read_log(int cookie){
    //0 = storage
    //1 = username
    //2 = phone
    //3 = password
    char *filename[] = {"config.txt","log-username.txt","log-phone.txt","log-password.txt"};
    FILE *fp = fopen(filename[cookie], "r");
    if (fp == NULL)
    {
        printf("Error: could not open file %s", filename[cookie]);
        return 1;
    }
    // reading line by line, max 256 bytes
    int i = 1;
    const unsigned MAX_LENGTH = 256;
    char buffer[MAX_LENGTH];
    //
    switch(cookie){
        case 0://storage
        while (fscanf(fp, "%d", &login[i])==1){
        ++i;
        }
        break;
        //
        case 1://username
        while (fgets(buffer, MAX_LENGTH, fp)) {
        buffer[strcspn(buffer, "\n")] = 0;
        log_username[i] = strdup(buffer); // Allocate memory and copy string
        if (log_username[i] == NULL) {
            perror("Error allocating memory");
            return 1;
        }
        ++i;
        ++checker1;
        ++current_user;
        }
        break;
        //
        case 2://phone
        while (fgets(buffer, MAX_LENGTH, fp)){
        buffer[strcspn(buffer, "\n")] = 0;
        log_phone[i] = strdup(buffer); // Allocate memory and copy string
        if (log_phone[i] == NULL) {
            perror("Error allocating memory");
            return 1;
        }
        ++i;
        ++checker2;
        }
        break;
        //
        case 3://password
        while (fgets(buffer, MAX_LENGTH, fp)) {
        buffer[strcspn(buffer, "\n")] = 0;
        log_password[i] = strdup(buffer); // Allocate memory and copy string
        if (log_password[i] == NULL) {
            perror("Error allocating memory");
            return 1;
        }
        ++i;
        ++checker3;
        }
        break;
    }
    // close the file
    fclose(fp);
}

void write_log(int cookie, int fcase, int delete_ac){
    //*cookie
    //0 = storage
    //1 = username
    //2 = phone
    //3 = password
    //?fcase
    //0 = non delete ac service
    //1 = delete ac service activated
    //?delete_ac => user that gonna delete
            char *filename[] = {"config.txt","log-username.txt","log-phone.txt","log-password.txt"};
             // open the file for writing
            FILE *fp = fopen(filename[cookie], "w");
            if (fp == NULL){
            printf("Error opening the file %s", filename[cookie]);
                return -1;
            }
                // write to the text file
                switch(cookie){
                    case 0:
                    if(fcase == 1){
                        login[1] = 0;
                        login[2] = 0;
                    }
                    fprintf(fp ,"%d\n%d", login[1],login[2]);
                    break;
                    case 1:
                    for(int i=1;i<=current_user;i++){
                        if(fcase == 1 && delete_ac == i){fcase = 0;++current_user;continue;}
                        if(i==current_user){
                            fprintf(fp ,"%s", log_username[i]);
                            break;
                        }
                        fprintf(fp ,"%s\n", log_username[i]);
                    }
                    break;
                    case 2:
                    for(int i=1;i<=current_user;i++){
                        if(fcase == 1 && delete_ac == i){fcase = 0;continue;}
                        if(i==current_user){
                            fprintf(fp ,"%s", log_phone[i]);
                            break;
                        }
                        fprintf(fp ,"%s\n", log_phone[i]);
                    }
                    break;
                    case 3:
                    for(int i=1;i<=current_user;i++){
                        if(fcase == 1 && delete_ac == i){fcase = 0;continue;}
                        if(i==current_user){
                            fprintf(fp ,"%s", log_password[i]);
                            break;
                        }
                        fprintf(fp ,"%s\n", log_password[i]);
                    }
                    break;
                }
                // close the file 
                fclose(fp);
}

int check_source(int ad){
    //checking_user = 1;// none used
    int temp = 1;
    switch(ad){
        case 1:
        for(int i = 1;i<=current_user;i++){
            temp = strcmp(username,log_username[i]);
            if(temp == 0){
                break;
            }
        }
        break;
        case 2:
        for(int i = 1;i<=current_user;i++){
            temp = strcmp(phone,log_phone[i]);
            if(temp == 0){
                break;
            }
        }
        break;
        case 3:
        for(int i = 1;i<=current_user;i++){
            temp = strcmp(password,log_password[i]);
            if(temp == 0){
                break;
            }
        }
        break;
        printf("\n%d\n", temp);
    }
    return temp;
}

void loadingBar(int type)
{
    system("cls");
	char a = 177, b = 219;
	printf("\n\n\n\n\n\n\t\t\t\t\t");
    switch (type)
    {
    case 1:
        printf("Loading......\n\n");
        break;
    case 2:
        printf("Saving......\n\n");
        break;
    case 3:
        printf("Setting up......\n\n");
    }
	printf("\t\t\t\t\t");
	// Print initial loading bar
	for (int i = 0; i < 26; i++)
		printf("%c", a);
	printf("\r");
	printf("\t\t\t\t\t");
	// Print loading bar progress
	switch (type)
    {
    case 1:
        for (int i = 0; i < 26; i++) {
		    printf("%c", b);
		    // Sleep for 0.003 second
		    Sleep(20);
	    }
        break;
    case 2:
        for (int i = 0; i < 26; i++) {
		    printf("%c", b);
		    // Sleep for 0.003 second
		    Sleep(30);
	    }
        break;
    case 3:
        for (int i = 0; i < 26; i++) {
		    printf("%c", b);
		    // Sleep for 0.003 second
		    Sleep(10);
	    }
        break;
    }
	system("cls");
}

int main(){
	loadingBar(1);
	printf("\n\n\n\n");
	printf("\nSaki");
    for(int i = 0; i<=3;++i){
        read_log(i);
    }
    system("cls");//!clear
    //printf("%d | %d | %d\n", login[1],current_user,checker1);//debugger
    if(MAX_USER == 0){
        printf("\nMAX_USER in setting can't be %d. It should only be more than 0 \n", MAX_USER);
        printf("\nProgram was closed <Confirm the setting before continue>\n");
        exit(0);
    }
    if(MAX_STRING < 16){
        printf("\nMAX_SRTING in setting can't be %d. It should only be more than 15 \n", MAX_STRING);
        printf("\nProgram was closed <Confirm the setting before continue>\n");
        exit(0);
    }
    if(current_user > MAX_USER || checker1 > MAX_USER || checker1 < 0 || current_user < 0){
        printf("\nPlease check setting in code.\nThere is something wrong with MAX_USER to setup array\nMAX_USER : %d\nUsers in slots: ERROR! (Can't read)\n!Incompatible!\n", MAX_USER);
        printf("\nProgram was closed <Confirm the setting before continue>\n");
        exit(0);
    }
	setup();
}
//---------------------------------
void setup(){//*setup everything
	// 1 = logged in | 0 = No account be logged in
	switch(login[1]){
    case 0:
        register_service();
        break;
    case 1:
        logged_in();
        break;
	}
}
//---------------------------------

void register_service(){//*login = 0 | choice to sign up or sign in
    char choice = 0, c1 = 0;
    while(1){
        if(checker1 != checker2 || checker1 != checker3 || checker2 != checker3){
            printf("\n!Error packaging slots!\nIf you're still using , datas may go wrong or losing some datas.\nContract Saki for any information | DISCORD ID : @Kotori_Saki\n");
            printf("Username in slots : %d\nPhone-number in slots : %d\nPassword in slots: %d\n\n", checker1, checker2, checker3);
        }
        printf("Welcome to Login Services");
        printf("\n-------------------");
        printf("\n1) Sign up");
        printf("\n2) Sign in");
        printf("\n3) Close program");
        printf("\n4) Credits & System information");
        printf("\n=> ");//input choice
        scanf(" %c", &choice);
        if(choice== '1' || choice== '2' || choice == '3' || choice == '4'){
            break;
        }
        choice = '0';
        system("cls");//!clear
        printf("\n!choose only 1 , 2 , 3!\n");
    }
        switch(choice){
        case '1':
            if(current_user >= MAX_USER){//to set current user not be more than arrays
                system("cls");//!clear
                printf("\n!It's now full user accounts in slots! \ncurrent user : %d\n\n",current_user);
                setup();
            }
            choice = 0;
            system("cls");//!clear
            sign_up();//send to void
            break;
        case '2':
            choice = 0;
            if(checker1 == 0 || checker2 == 0 || checker3 == 0 || current_user == 0){
                system("cls");//!clear
                printf("!There is no any account yet!\nPlease sign up first\n\n");
                setup();
                break;
            }
            system("cls");//!clear
            sign_in();//send to void
            break;
        case '3':
            choice = 0;
            system("cls");//!clear
            exit_point();
            break;
        case '4':
            choice = 0;
            system("cls");//!clear
            printf("- Kitatrist Riabroi <Saki> => Coding(Main) | Concept Creation(Main) | Flowchart\n- Jittima Ladmool <AG> => Researching | Coding | Flowchart | Slideshow\n- Boonsiri Kamonchatsakul <THA-YAH> => Flowchart(Main) | Slideshow\n- Kongphop Kanpai <MickMick> => Slideshow | Flowchart\n- Pratchaya Saekram <JUNIOR> => Slideshow(Main)\n\n\n");
            printf("User count in slots : %d\n", current_user);
            printf("Max available user count in slots : %d\n", MAX_USER);
            printf("User slots left : %d\n", MAX_USER-current_user);
            printf("MAX_STRING: %d\n", MAX_STRING);
            printf("\n\nPress ENTER on keyboard to continue...");
            getchar();//Clear buffer
            getchar();
            system("cls");//!clear
            setup();
            break;
        }
        choice = 0;
}

void sign_up(){
    // input : username , email , password
    int len_phone = 0, temp100=0;
    //
    char a,b,c1,c2,c3;//choices
    int x=0,y=0,z=0;//counting temp
    while(1){
        printf("!Requirement!\n");
        printf("Username => input without ' ' and around 6 - 16 letters\n");
        printf("Phone => input with 0 in front; Example : 0987654321\n");
        printf("Password => input without ' ' and around 8 - 16 lettes\n");
        printf("\nAccept? (y/n) => ");
        scanf(" %c", &a);
        if(a == 'y' || a == 'n'){
            printf("\n");
            system("cls");//!clear
            break;
        }
        system("cls");//!clear
        printf("\nError input , try again\n");
    }
    if(a == 'n'){system("cls");setup();}
                //Improved to check every input before continue
                while(1){//input username
                    if(x>=2){printf("\n!Seriously!\n\n");}
                    if(x>=3){
                        while(1){
                            printf("\nWanna try more? (y/n)=>");
                            scanf(" %c", &c1);
                            if(c1 == 'y' || c1 == 'n'){
                                break;
                            }
                            system("cls");//!clear
                        }
                        system("cls");//!clear
                        if(c1 == 'y'){sign_up();}
                        if(c1 == 'n'){setup();}
                        x = 0;
                    }
                    printf("!Requirement!\n");
                    printf("Username => input without ' ' and around 6 - 16 letters\n");
                    printf("Phone => input with 0 in front; Example : 0987654321\n");
                    printf("Password => input without ' ' and around 8 - 16 lettes\n\n");
                    printf("New Username => ");
                    scanf(" %s", &username);
                    x += 1;
                    if(strlen(username)<6 || strlen(username)>16){//!failure
                        system("cls");//!clear
                        printf("please follow this => input without ' ' and around 6 - 16 letters\nYou did only %d characters\ntry again\n\n", strlen(username));
                        continue;
                    }
                    if(check_source(1) == 0){//!failure
                        system("cls");//!clear
                        printf("Someone already use this username\nplease try again\n\n");
                        continue;
                    }
                    if(strlen(username)>=6 && strlen(username)<=16 && check_source(1) != 0){//?complete
                        printf("Nice!\n\n");
                        break;
                    }
                }
                while(1){//input phone
                    if(y>=2){printf("\n!Seriously!\n\n");}
                    if(y>=3){
                        system("cls");//!clear
                        while(1){
                            printf("Wanna try more? (y/n)=>");
                            scanf(" %c", &c2);
                            if(c2 == 'y' || c2 == 'n'){
                                break;
                            }
                            system("cls");//!clear
                        }
                        system("cls");//!clear
                        if(c2 == 'y'){sign_up();}
                        if(c2 == 'n'){setup();}
                        y = 0;
                    }
                    printf("New phone-number => ");
                    scanf(" %s", &phone);
                    y += 1;
                    temp100 = 0;
                    len_phone = strlen(phone);
                    for(int i = 0; i < len_phone; ++i){//!To check string is "1 - 9"
                        if(phone[i] < '0' || phone[i] > '9'){
                            temp100 = 1;
                        }
                    }
                    if(strlen(phone)!=10 || temp100 == 1){//!failure
                        system("cls");//!clear
                        printf("please follow this => input with 0 in front; Example : 0987654321\ntry again\n\n");
                        printf("New Username => %s\n", username);
                        continue;
                    }
                    if(check_source(2) == 0){//!failure
                        system("cls");//!clear
                        printf("Someone already use this phone-number\ntry again\n\n");
                        printf("New Username => %s\n", username);
                        continue;
                    }
                    if(strlen(phone)==10 && check_source(2) != 0){//?complete
                        printf("Got your order!\n\n");
                        break;
                    }
                }
                while(1){//input password
                    if(z>=2){printf("\n!Seriously!\n\n");}
                    if(z>=3){
                        system("cls");//!clear
                        while(1){
                            printf("Wanna try more? (y/n)=>");
                            scanf(" %c", &c3);
                            if(c3 == 'y' || c3 == 'n'){
                                break;
                            }
                            system("cls");//!clear
                        }
                        system("cls");//!clear
                        if(c3 == 'y'){sign_up();}
                        if(c3 == 'n'){setup();}
                        z = 0;
                    }
                    printf("New Password => ");
                    scanf(" %s", &password);
                    z += 1;
                    if(strlen(password)<8 || strlen(password)>16){//!failure
                        system("cls");//!clear
                        printf("please follow this => input without ' ' and around 8 - 16 lettes\nYou did %d characters\ntry again\n\n", strlen(password));
                        printf("New Username => %s\n", username);
                        printf("New phone-number => %s\n", phone);
                        continue;
                    }
                    if(strlen(password)>=8 && strlen(password)<=16){//?complete
                        //printf("\nComplete\n");
                        break;
                    }
                }
            ++current_user;
            login[1] = 1;
            login[2] = current_user;
            log_username[current_user] = username;
            log_phone[current_user] = phone;
            log_password[current_user] = password;
            loadingBar(2);
            write_log(0,0,0);
            write_log(1,0,0);
            write_log(2,0,0);
            write_log(3,0,0);
            new_user = 1;
            for(int f = 1; f <= current_user; ++f){//!reset
                log_username[f] = "(null)";
                log_phone[f] = "(null)";
                log_password[f] = "(null)";
            }
            loadingBar(3);
            current_user = 0;//!reset
            for(int i = 1; i<=3;++i){//?recovery logs
                read_log(i);
            }
            setup();
}

void sign_in(){
    // input : username , password
    system("cls");//!clear
    int checking_user=0;
    char a = 'a';
    while(1){
        printf("\nConfirm (y/n) => ");
        scanf(" %c", &a);
        if(a == 'y' || a == 'n'){
            system("cls");//!clear
            break;
        }
        system("cls");//!clear
        printf("\nError input , try again\n");
    }
    if(a == 'n'){system("cls");setup();}
    int temp1, temp2, temp, i;
    temp1 = 0;
    temp2 = 0;
    //checking_user = 0;
    int i_know_you = 0;
    int check1;
            while(1){
                ++i_know_you;
                temp = 1, temp1=0, temp2=1;
                check1 = 0;
                printf("\nTo login, Enter your account informations\n");
                printf("\nEnter Username => ");
                scanf(" %s", &username);
                printf("Enter Password => ");
                scanf(" %s", &password);
                for(i = 1;i<=current_user;i++){//!Old system checking array by array
                    temp = strcmp(username,log_username[i]);
                    if(temp == 0){
                        temp1 = i;
                        checking_user = i;
                        temp = 1;
                        temp = strcmp(password,log_password[checking_user]);
                        if(temp == 0){//!New system to focus check to array from username array
                        temp2 = 0;
                        }
                        system("cls");//!clear
                        break;
                    }
                    system("cls");//!clear
                }
                //printf("\n%d | %d\n", temp1, temp2);//*debugger
                if(temp1 != 0 && temp2 == 0){break;}// check if check1 = 3
                if(i_know_you >= 3){
                    char choice = 'a';
                    printf("\nSeem like you forgot your username or password\n");
                    while(1){
                        printf("wanna get some help (y/n)=> ");
                        scanf(" %c", &choice);
                        if(choice == 'y'){
                            system("cls");//!clear
                            forget_password();
                        }
                        if(choice == 'n'){
                            system("cls");//!clear
                            setup();
                        }
                        system("cls");//!clear
                    }
                    printf("\nI know that!\n");
                }
                printf("Error input, please check again and\nMaybe it cause no user account in slot\n\n");
            }
            //printf("\n%d | %d\n", temp1, temp2);//*debugger
            login[1] = 1;
            login[2] = temp1;
            loadingBar(3);
            write_log(0,0,0);
            setup();
}
 
void logged_in(){
    char choice = 0;
    char choice1;
    while(1){
        if(new_user == 1){
            printf("Welcome! , %s!\n------------------------\n", log_username[login[2]]);
            new_user = 0;
        }
        else{
            printf("Welcome back! , %s!\n------------------------\n", log_username[login[2]]);
        }
        printf("Option\n1) Sign out\n2) Delete account\n3) Close program\n=> ");
        scanf(" %c", &choice);
        if(choice == '1' || choice == '2' || choice == '3'){
            break;
        }
        system("cls");//!clear
        printf("\nChoose only 1,2,3\n");
    }
    int x;
    switch(choice){
        case '1':
        login[1] = 0;
        login[2] = 0;
        write_log(0,0,0);
        choice = 0;
        loadingBar(3);
        setup();
        break;
        //
        case '2'://Deleting account
        x = login[2];
        --current_user;
        loadingBar(2);
        write_log(0,1,x);
        write_log(1,1,x);
        write_log(2,1,x);
        write_log(3,1,x);
        for(int f = 1; f <= current_user; ++f){//reset datas in arrays
                log_username[f] = "(null)";
                log_phone[f] = "(null)";
                log_password[f] = "(null)";
        }
        current_user = 0;//!reset
        for(int i = 1; i<=3;++i){//?recovery logs
            read_log(i);
        }
        setup();
        break;
        //
        case '3':
        choice = 0;
        exit_point();
    }
}

void forget_password(){
    int check1, check2, temp1, temp2, i, f, count=0;
    while(1){
        check1 = 0;
        check2 = 0;
        printf("\nPlease enter your phone-number that was registered => ");
        scanf("%s", &phone);
        printf("What is your username that connect to your account? => ");
        scanf("%s", &username);
        //-----------------------
        for(i = 1; i<=current_user ;++i){
            temp1 = 1;
            temp1 = strcmp(phone,log_phone[i]);
            if(temp1 == 0){
                check1 = i;
            }
        }
        for(i = 1; i<=current_user ;++i){
            temp2 = 1;
            temp2 = strcmp(username,log_username[i]);
            if(temp2 == 0){
                check2 = i;
            }
        }
        ++count;
        if(count >= 3){
            char choice = 'a';
            printf("\nSeem like you forgot your phone-number or username\n");
            while(1){
                printf("wanna get some help (y/n)=> ");
                scanf(" %c", &choice);
                if(choice == 'y'){
                    system("cls");//!clear
                    forget_password();
                }
                if(choice == 'n'){
                    system("cls");//!clear
                    setup();
                }
                system("cls");//!clear
            }
            printf("\nI know that!\n");
        }
        //printf("\n%d | %d\n", check1, check2);//*debugger
        if(check1 == check2 && check2 != 0){
            f = check2;
            system("cls");//!clear
            printf("\nYoue account be found and user is allowed to change user's password\n");
            change_details(f);
        }
        system("cls");//!clear
        printf("Account is not found. Try again\n");
    }
}

void change_details(int user){
    int g,h=0;
    while(1){
        g = 0;
        printf("\nPassword : input without ' ' and around 8 - 16 lettes\n");
        printf("ID : %s\n", log_username[user]);
        printf("Enter new password => ");
        scanf("%s", &password);
        ++h;
        if(strlen(password)>=8 && strlen(password)<=16){
            printf("\nDone1\n");
            g = 1;
        }
        if(g == 1){break;}
        system("cls");//!clear
        if(h>=3){printf("\n!Seriously!\n");}
        printf("\nSomething wrong, try again\n");
    }
    log_password[user] = password;
    loadingBar(2);
    printf("\nComplete changed! Try to login again\n");
    write_log(3,0,0);
    for(int f = 1; f <= current_user; ++f){//!reset
        log_username[f] = "(null)";
        log_phone[f] = "(null)";
        log_password[f] = "(null)";
    }
    current_user = 0;//!reset
    for(int i = 1; i<=3;++i){//?recovery logs
        read_log(i);
    }
    setup();
}