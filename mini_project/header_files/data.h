//PORT NO
#define PORT_NO 8081    


#define INITIAL_PROMPT "Welcome to bank apllication!\nlogin as: (Enter your choice)\n1. Admin\t2. Customer\t3. Manager \t4. Bank employee\tPress any other number to exit\nEnter the number corresponding to the choice!\n"

#define ADMIN_LOGIN_WELCOME_MSG "Welcome Admin! \nEnter your credentials to unlock !"
#define ADMIN_LOGIN_SUCCESS "Welcome Admin!"




// LOGIN
#define LOGIN_ID "Enter your login ID :"
#define PASSWORD "Enter your password : "
#define INVALID_LOGIN "The login ID specified doesn't exist!$"
#define INVALID_PASSWORD "The password specified doesn't match!$"


// ADMIN MENU
#define ADMIN_MENU "\n1.Add customer\n2. Get Customer Details \n3. Add User(manager/employee)\n4. Modify Customer Information\n5. Change Password.\n7. Logout\n8. Add Account \n 9. Change user role \n 10 .Modify user information \n Press any other key to logout "


//CUSTOMER MENU
#define CUSTOMER_MENU "\n1.View Account Balance\n 2.Deposit Money\n 3.Withdraw Money\n 4.Transfer Funds \n 5.Apply for a Loan \n 6.Change Password \n 7.Adding Feedback \n 8.View Transaction History \n 9.Logout \n Press any other key to logout "


//MANAGER MENU
#define MANAGER_MENU "\n1.Activate/Deactivate Customer Account \n2.Assign Loan Application Processes to Employees \n3.Review Customer Feedback \n4.Change Password \n5.View Account details \n6.Logout \n Press any other key to logout "
#define MANAGER_LOGIN_SUCCESS "Welcome Manager! \nEnter your credentials to unlock !"

//EMPLOYEE MENU
#define EMPLOYEE_LOGIN_SUCCESS "Welcome Employee! \nEnter your credentials to unlock !"
#define EMPLOYEE_MENU "\n1.Add New Customer \n2.Modify Customer Details \n3.Approve/Reject Loans \n4.View Assigned Loan Applications \n5.View Customer Transactions \n6.Change Password \n7.Logout \n Press any other key to logout "

//ADD CUSTOMERo
///#define ADMIN_ADD_CUSTOMER_PRIMARY "Enter the details for the primary customer\n"
//#define ADMIN_ADD_CUSTOMER_SECONDARY "Enter the details for the secondary customer\n"
#define ADMIN_ADD_CUSTOMER_NAME "Enter Customer name :"
#define ADMIN_ADD_CUSTOMER_GENDER "Enter Customer gender F,M or O (only) :"
#define ADMIN_ADD_CUSTOMER_AGE "Enter customer's age  :"
//#define ADMIN_ADD_CUSTOMER_AUTOGEN_LOGIN "The autogenerated login ID for the customer is : "
///#define AUTOGEN_PASSWORD "spookytime" // append to end of next string macro

#define ADMIN_ADD_CUSTOMER_AUTOGEN_LOGIN "The autogenerated login for the customer is :"
#define ADMIN_ADD_CUSTOMER_AUTOGEN_PASSWORD "The autogenerated password for the customer is : "
#define ADMIN_ADD_CUSTOMER_WRONG_GENDER "It seems you've enter a wrong gender choice!\nYou'll now be redirected to the main menu!^"
#define ADMIN_ADD_ACCOUNT_NUMBER "Enter customer account number :"
#define ADMIN_ADD_EMAIL_ID "Enter customer Email Address :"



//ADD USER

#define ADMIN_ADD_USER_NAME "Enter User name :"
#define ADMIN_ADD_USER_GENDER "Enter User gender F,M or O (only) :"
#define ADMIN_ADD_USER_AGE "Enter User age :"
//#define ADMIN_ADD_User_AUTOGEN_LOGIN "The autogenerated login ID for the User is : "
///#define AUTOGEN_PASSWORD "spookytime" // append to end of next string macro
#define ADMIN_ADD_USER_AUTOGEN_PASSWORD "The autogenerated password for the User is : "
#define ADMIN_ADD_USER_WRONG_GENDER "It seems you've enter a wrong gender choice!\nYou'll now be redirected to the main menu!^"
//#define ADMIN_ADD_ACCOUNT_NUMBER "Enter User account number"
#define ADMIN_ADD_USER_EMAIL_ID "Enter User Email Address :"
// #define ADMIN_ADD_USER_ISACTIVE "Enter whether "
#define ADMIN_ADD_USER_ROLE "Enter User's Role :"


//RESULT 
#define LOGIN_ID_IS "Your Login id is:"
#define LOGIN_PASS_IS "Your Login Password is:"
#define LOGIN_SUCCESSFULL_MSG "Login is done successfully!!@"
#define LOGIN_SUCCESSFULL_MSG_ADMIN "Login is done successfully!!"


//MODIFYING CUSTOMER DETAIL
#define ADMIN_MOD_CUSTOMER_MENU "Which information would you like to modify?\n1. Name 2. Age 3. Gender \nPress any other key to cancel"
#define ADMIN_MOD_EMPLOYEE_MENU "Which information would you like to modify?\n1. Name 2. Age 3. Gender \nPress any other key to cancel"

#define ADMIN_MOD_CUSTOMER_ID "Enter the ID of the customer who's information you want to edit"
#define ADMIN_MOD_CUSTOMER_NEW_NAME "What's the updated value for name?"
#define ADMIN_MOD_CUSTOMER_NEW_GENDER "What's the updated value for gender?"
#define ADMIN_MOD_CUSTOMER_NEW_AGE "What's the updated value for age?"

#define ADMIN_MOD_CUSTOMER_SUCCESS "The required modification was successfully made!\nYou'll now be redirected to the main menu!^"



//MODIFYING USER DETAIL
#define ADMIN_MOD_USER_MENU "Which information would you like to modify?\n1. Name 2. Age 3. Gender \nPress any other key to cancel"
//#define ADMIN_MOD_EMPLOYEE_MENU "Which information would you like to modify?\n1. Name 2. Age 3. Gender \nPress any other key to cancel"

#define ADMIN_MOD_USER_ID "Enter the ID of the customer who's information you want to edit"
#define ADMIN_MOD_USER_NEW_NAME "What's the updated value for name?"
#define ADMIN_MOD_USER_NEW_GENDER "What's the updated value for gender?"
#define ADMIN_MOD_USER_NEW_AGE "What's the updated value for age?"

#define ADMIN_MOD_USER_SUCCESS "The required modification was successfully made!\nYou'll now be redirected to the main menu!^"
#define USER_ID_DOESNT_EXIT "No customer is there for the given CUSTOMER_ID"


// LOGIN
#define LOGIN_ID_MSG "Enter your login ID\n"
#define PASSWORD_MSG "Enter your password \n"
#define INVALID_LOGIN_MSG "The login ID specified doesn't exist!$"
#define INVALID_PASSWORD_MSG "The password specified doesn't match!$"


//ADD ACCOUNT
#define ACCOUNT_NUMBER_ADDED "Your newly created account's number is :"
#define ACCOUNT_ID_DOESNT_EXIST "Account id does not exist :"
#define GET_ACCOUNT_NUMBER "Enter the account number of the account you're searching for"
#define ACCOUNT_DEACTIVATED "This account is not activate."

//CUSTOMER MENU

#define CUSTOMER_LOGIN_WELCOME "Welcome dear customer! Enter your credentials to gain access to your account!"
#define CUSTOMER_LOGIN_SUCCESS "Welcome beloved customer!"





#define USER_LOGOUT "Logging Out!$"
#define CUSTOMER_LOGOUT "Logging Out!$"
#define GET_CUSTOMER_ID_MSG "Enter the customer ID of the customer you're searching for"
#define CUSTOMER_ID_DOESNT_EXIT "No customer is there for the given CUSTOMER_ID"

//WITHDRAW AMOUNT

#define WITHDRAW_AMOUNT "How much is it that you want to withdraw from your bank?\n"
#define WITHDRAW_AMOUNT_INVALID "You seem to have either passed an invalid amount or you don't have enough money in your bank to withdraw the specified amount^"
#define WITHDRAW_AMOUNT_SUCCESS "The specified amount has been successfully withdrawn from your bank account!^"


//DEPOSIT AMOUNT
#define DEPOSIT_AMOUNT "How much amount you want to add into your bank?\n"
#define DEPOSIT_AMOUNT_INVALID "You have entered an invalid amount!^"
#define DEPOSIT_AMOUNT_SUCCESS " Specified amount has been successfully added to your bank account!^"
#define ACCOUNT_NOT_FOUND "Sorry,we are unable to get account num.^"


//TRANSFER FUNDS
#define ENTER_RECIPIENT_ACCOUNT "Enter recipient account number art which you want to transfer the funds: "
#define ENTER_TRANSFER_AMOUNT "Enter the amount you want to transfer :"
#define TRANSFER_SUCCESS "Specified amount is transfered successfully :^"
#define TRANSFER_INVALID "Amount you are transfering is invalid :"


//ADDING FEEDBACK
#define ADD_FEEDBACK "Enter feedback details.\n"


//PASSWORD CHANGE
#define PASSWORD_CHANGE_OLD_PASS "Enter your old password"
#define PASSWORD_CHANGE_OLD_PASS_INVALID "The entered password doesn't seem to match with the old password"
#define PASSWORD_CHANGE_NEW_PASS "Enter the new password"
#define PASSWORD_CHANGE_NEW_PASS_RE "Reenter the new password"
#define PASSWORD_CHANGE_NEW_PASS_INVALID "The new password and the reentered passwords don't seem to pass!^"
#define PASSWORD_CHANGE_SUCCESS "Password successfully changed!^"


//INVALID CHOICES
#define ERRON_INPUT_FOR_NUMBER "It seems you have passed a sequence of alphabets when a number was expected or you have entered an invalid number!\nYou'll now be redirected to the main menu!^"
#define INVALID_MENU_CHOICE "It seems you've made an invalid menu choice\nYou'll now be redirected to the main menu!^"


#define FEEDBACK_SUCCESS "Feedback is added successfully^"
#define TRANSACTIONS_NOT_FOUND "There is no transaction on thias account^"
#define USER_LOGIN_WELCOME "Welcome User" 


//MANAGER DETAILS
#define ENTER_LOAN_ID "Enter a loan id to assign :"
#define ENTER_EMPLOYEE_ID "Enter a employee  id to which you want to assign a loan :"
#define LOAN_ID_ASSIGN_SUCCESS "Loan ID  assigned to Employee ID  successfully^"
#define LOAN_ID_NOTFOUND "Loan ID not found .^"
#define EMPLOYEE_ID_NOTFOUND "Employee ID not found .^" 
// #define USER_LOGIN_WELCOME "Welcome User"

//EMPLOYEE DETAILS
#define APPROVE_REJECT "Enter 1 to approve the loan,Press 0 to reject the loan :"


//HASH SIZE
#define HASH_SIZE 64

#define ACCOUNT_FILE "/home/aayushi312000/MTech/SS/Banking-management/mini_project/stored_data/account_records.txt"
#define CUSTOMER_FILE "/home/aayushi312000/MTech/SS/Banking-management/mini_project/stored_data/customer_records.txt"
#define USER_FILE "/home/aayushi312000/MTech/SS/Banking-management/mini_project/stored_data/user_records.txt"
#define TRANSACTION_FILE "/home/aayushi312000/MTech/SS/Banking-management/mini_project/stored_data/transaction_records.data"
#define BANK_EMPLOYEE_FILE "/home/aayushi312000/MTech/SS/Banking-management/mini_project/stored_data/employee_records.txt"
#define FEEDBACK_FILE "/home/aayushi312000/MTech/SS/Banking-management/mini_project/stored_data/feesback_records.txt"
#define LOAN_FILE "/home/aayushi312000/MTech/SS/Banking-management/mini_project/stored_data/loan_records.txt"