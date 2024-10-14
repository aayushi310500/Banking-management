//PORT NO
#define PORT_NO 8081


#define INITIAL_PROMPT "Welcome to bank apllication!\nlogin as: (Enter your choice)\n1. Admin\t2. Customer\t3. Manager \t4. Bank employee\nPress any other number to exit\nEnter the number corresponding to the choice!\n"

#define ADMIN_LOGIN_WELCOME_MSG "Welcome Admin! \nEnter your credentials to unlock !"
#define ADMIN_LOGIN_SUCCESS "Welcome Admin!"


// ADMIN MENU
#define ADMIN_MENU "1.Add Customer\n2. Get Customer Details\n3. Get Account Details\n4. Get Transaction details\n5. Add Account\n6. Delete Account\n7. Modify Customer Information\n8. temp braek while loop\n Press any other key to logout"


//ADD CUSTOMER
///#define ADMIN_ADD_CUSTOMER_PRIMARY "Enter the details for the primary customer\n"
//#define ADMIN_ADD_CUSTOMER_SECONDARY "Enter the details for the secondary customer\n"
#define ADMIN_ADD_CUSTOMER_NAME "Enter Customer name"
#define ADMIN_ADD_CUSTOMER_GENDER "Enter Customer gender F,M or O (only)"
#define ADMIN_ADD_CUSTOMER_AGE "What is the customer's age?"
//#define ADMIN_ADD_CUSTOMER_AUTOGEN_LOGIN "The autogenerated login ID for the customer is : "
///#define AUTOGEN_PASSWORD "spookytime" // append to end of next string macro
#define ADMIN_ADD_CUSTOMER_AUTOGEN_PASSWORD "The autogenerated password for the customer is : "
#define ADMIN_ADD_CUSTOMER_WRONG_GENDER "It seems you've enter a wrong gender choice!\nYou'll now be redirected to the main menu!^"
#define ADMIN_ADD_ACCOUNT_NUMBER "Enter customer account number"
#define ADMIN_ADD_EMAIL_ID "Enter customer Email Address"


//RESULT 
#define LOGIN_ID_IS "Your Login id is:"
#define LOGIN_PASS_IS "Your Login Password is:"
#define LOGIN_SUCCESSFULL_MSG "Login is done successfully!!"


//MODIFYING CUSTOMER DETAIL
#define ADMIN_MOD_CUSTOMER_MENU "Which information would you like to modify?\n1. Name 2. Age 3. Gender \nPress any other key to cancel"
#define ADMIN_MOD_EMPLOYEE_MENU "Which information would you like to modify?\n1. Name 2. Age 3. Gender \nPress any other key to cancel"

#define ADMIN_MOD_CUSTOMER_ID "Enter the ID of the customer who's information you want to edit"
#define ADMIN_MOD_CUSTOMER_NEW_NAME "What's the updated value for name?"
#define ADMIN_MOD_CUSTOMER_NEW_GENDER "What's the updated value for gender?"
#define ADMIN_MOD_CUSTOMER_NEW_AGE "What's the updated value for age?"

#define ADMIN_MOD_CUSTOMER_SUCCESS "The required modification was successfully made!\nYou'll now be redirected to the main menu!^"

// LOGIN
#define LOGIN_ID_MSG "Enter your login ID\n"
#define PASSWORD_MSG "Enter your password \n"
#define INVALID_LOGIN_MSG "The login ID specified doesn't exist!$"
#define INVALID_PASSWORD_MSG "The password specified doesn't match!$"


//ADD ACCOUNT
#define ACCOUNT_NUMBER_ADDED "Your newly created account's number is :"


#define CUSTOMER_LOGOUT "Logging Out!$"
#define GET_CUSTOMER_ID_MSG "Enter the customer ID of the customer you're searching for"
#define CUSTOMER_ID_DOESNT_EXIT "No customer is there for the given CUSTOMER_ID"

//INVALID CHOICES
#define ERRON_INPUT_FOR_NUMBER "It seems you have passed a sequence of alphabets when a number was expected or you have entered an invalid number!\nYou'll now be redirected to the main menu!^"
#define INVALID_MENU_CHOICE "It seems you've made an invalid menu choice\nYou'll now be redirected to the main menu!^"


//HASH SIZE
#define HASH_SIZE 64

#define ACCOUNT_FILE "/home/aayushi312000/MTech/SS/Banking-management/mini_project/stored_data/account_records.txt"
#define CUSTOMER_FILE "/home/aayushi312000/MTech/SS/Banking-management/mini_project/stored_data/customer_records.txt"
#define TRANSACTION_FILE "/home/aayushi312000/MTech/SS/Banking-management/mini_project/stored_data/transaction_records.txt"
#define BANK_EMPLOYEE_FILE "/home/aayushi312000/MTech/SS/Banking-management/mini_project/stored_data/employee_records.txt"