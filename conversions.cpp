// @file conversions.cpp
// @author Cameron Shore
// @date 15-Jun-2020
// This file holds declarations and definitions for conversion program.
//
// This program is a CLI based reprensentation of a conversion calculator being
// able to convert between hex, dec, and binary numbers.
// This also serves as a model for GUI based calculator.
//
// Features:
//    Conversion between decimal, hexadecimal, and binary numbers.
//    String parsing and string manipulation to ensure correct input and correct output of 
//    numbers, and characters.
//    Error handling and checking to ensure program runs without fault or failure

#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <math.h>
using namespace std;

// index table for hex values
const int HEX_INTS[6] = {10, 11, 12, 13, 14, 15}; // bad practice: suggest passing as argument

// index table for hex characters
const char HEX_CHARS[6] = {'A', 'B', 'C', 'D', 'E', 'F'}; // bad practice

// index table for bin value - bad practice suggest passing as argument
const string BIN_STR[16] = {"0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111", 
                            "1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111"};

// Main method of the program, displays options for the user to choose.
// input is validated to ensure proper functioning of the program
// Only digits allowed in the main function
// only one argument allowed at time of development - subject to changed
// @return 0 on normal exit, otherwise specific error code is returned.
int main() {
   // Since this program is trivial forward declaration is fine - I should have used header file
   // really regretting not using a header, cpp and main file.... this is messy
   // forward declaration of conversion methods:
   void hexToDec();
   void binToDec();
   // void decToHex(); implement later
   // void decToBin(); implement later
   void hexToBin();
   void binToHex();
   char getHexChar(stack<char>);

   // variable declarations:

   // lower number of the choices can update if changes are made
   const char MIN_NUM_CHOICES = '1';

   // number of arguments needed for method to work properly
   const char MIN_NUM_ARGS = '1';

   // Number needed to exit the method NEEDS TO BE LARGEST NUMBER
   // PROGRAM USES THIS NUMBER AS UPPER BOUNDS CHECKING
   const int EXIT_NUM = 7;

   string input = ""; // holds input from user
   bool mainExit = false; // flag for ending the program

   

   do {
      // Display options
      cout << "1. Hexadecimal to Decimal Conversion" << endl;
      cout << "2. Binary to Decimal Conversion" << endl;
      cout << "3. Decimal to Hexadecimal Conversion (not supported currently)" << endl;
      cout << "4. Decimal to Binary Conversion (not supported currently)" << endl;
      cout << "5. Hexadecimal to Binary Conversion" << endl;
      cout << "6. Binary to Hexadecimal Conversion" << endl;
      cout << EXIT_NUM << ". Quit" << endl;
      cout << "Input number of option you wish to use : ";
      getline(cin, input);

      // input checking
      if (input[1] != '\0') { // ensure double digit isn't input or additional chars
         cerr << "Too many arguments in input. Input size: " 
               << MIN_NUM_ARGS << "\n"  << endl;
         cin.clear();
      }
      else if (isdigit(input[0]) == false) { // make sure input is actually a digit
         cerr << "Input is not a digit \n" << endl;
         cin.clear();
      }
      else if (input[0] < MIN_NUM_CHOICES || input[0] - '0' > EXIT_NUM) { //Bounds check
         cerr << "Input is out of bounds for the list \n" << endl;
         cin.clear();
      }
      else {
         const char exitNum = (char)EXIT_NUM + '0'; // create exit case
         switch (input[0]) {
         case '1': // Hexadecimal to Decimal
            hexToDec();
            break;
         case '2': // Binary to Decimal
            binToDec();
            break;
         case '3': // Decimal to Hexadecimal
            //decToHex();
            //break;
            cout << "Dec to Hex not supported at this time\n" << endl;
            continue;
         case '4': // Decimal to Binary
            //decToBin();
            //break;
            cout << "Dec to Bin not supported at this time\n" << endl;
            continue;
         case '5': // Hexadecimal to Binary
            hexToBin();
            break;
         case '6':
            binToHex();
            break;
         case exitNum: // exit case
            mainExit = true;
            break;
         default: // added security
            cerr << "Method Selection error has occured" << endl;
            return -1;
         }
      }
   } while (mainExit != true);
   return 0;
} // end main

// here are definitions of the methods, I should have put them in cpp, but I though this would 
// be simpler, turns out I'm wrong. Might change in the future.

char getHexChar(stack<char> binStack){
   int total = 0;
   char returnChar = ' ';
   for (int i = 0; i < 4; i++){
      if (binStack.top() == '1'){
         total += pow(2, i);
      }
      binStack.pop();
   }
   if (total > 9){
      returnChar = HEX_CHARS[total - 10];
   }
   else{
      returnChar = total + '0';
   }
   return returnChar;
}

// Conversts hexadecimal number to it's 32 bit int decimal eqivalent in a loop until exited
// String input is verified before proceeding
// Only hexadecimal digits accepted 0-9, A-F - Exit case checked first
// Automatically capitalizes input so any valid letter will be converted includes exit case
// upper bound of 8 digits (a byte), due to how easy it is to overflow with hexadecimal
// Overflow handler - based on 32 bit int.
// @return nothing - results printed to console
void hexToDec() {
   //variable declarations
   vector<char> hexVec; // maybe change to dynamic to allow mutiple instances
   bool validInput = true;
   bool hexToDecExit = false; // exit flag
   string exitString = "EXIT";
   string inputCheck = "";
   int total = 0;

   do {
      // reset variables to allow multiple loops
      validInput = true; // needed to reset flag to prevent forever false being set
      total = 0; // reset total
      hexVec.clear(); // clear

      cout << "\n" << "input 8 digit Hexadecimal number or \"EXIT\" to return to main menu: ";
      getline(cin, inputCheck);

      if (inputCheck.length() < 1){ // check empty string
         cerr << "no input detected" << endl;
         continue;
      }

      if (inputCheck.length() > 8){ // Due to how easy it is to overflow, just limiting numbers
         cerr << "8 bits (a byte) hexadecimal number supported" << endl;
         continue;
      }

      // force uppercase for easier string parsing
      for (int i = 0; i < inputCheck.length(); i++) { // probably a more effiecient 
         inputCheck[i] = toupper(inputCheck[i]);      // way to do this but it works
      }

      if (inputCheck.compare(exitString) == 0) { // check exit case
         return;
      }

      // validate input string
      for (string::reverse_iterator rit = inputCheck.rbegin(); rit != inputCheck.rend(); rit++) { // iterator practice
         if ((*rit >= '0' && *rit <= '9') || (*rit >= 'A' && *rit <= 'F')) {
            hexVec.push_back(*rit);
         }
         else if (*rit == 'X') { // cases where 0x was added front of hexdecimal number, just parses it off
            break;
         }
         else { // error message, clear vector, clear input, set flag to false
            cerr << "Invalid input: hexdecimal is 0-9 and A-F" << endl;
            validInput = false;
            hexVec.clear();
            cin.clear();
            break;
         }
      }

      if (validInput != true){
         continue;
      }
      else{ //here only 0-9 and A-F is possible
         for (int i = 0; i < hexVec.size(); i++){
            if (hexVec[i] >= 48 && hexVec[i] <= 57){
               total += ((int)hexVec[i] - '0') * pow(16, i);
            }
            else{
               total += HEX_INTS[hexVec[i] - 'A'] * pow(16, i);
            }
         }
         
         if(total < 0){ // best solution I could come up with (research better solutions)
            cerr << "Overflow Error max 32 bit signed binary int is 7FFFFFFF" << endl;
            continue;
         }
         cout << total << endl;
      }
   } while (hexToDecExit != true);
} // end hexToDec

// Conversts binary number to it's 32 bit int decimal eqivalent in a loop until exited
// String input is verified before proceeding
// Only Binary digits accepted 1's and 0's - Exit case checked first
// Automatically capitalizes input so any valid letter will be converted includes exit case
// Overflow handler - based on 32 bit int.
// @return nothing - results printed to console
void binToDec() {
   vector<char> binVec; // maybe change to dynamic to allow mutiple instances
   bool validInput = true;
   bool binToDecExit = false; // exit flag
   string exitString = "EXIT";
   string inputCheck = "";
   int total = 0;

   do {
      // reset variables to allow multiple loops
      validInput = true; // needed to reset flag to prevent forever false being set
      total = 0; // reset total
      binVec.clear(); // clear
      cout << "\n" << "input Binary number or \"EXIT\" to return to main menu: ";
      getline(cin, inputCheck);

      if (inputCheck.length() < 1){ // check empty string
         cerr << "no input detected" << endl;
         continue;
      }

      // force uppercase for easier string parsing
      for (int i = 0; i < inputCheck.length(); i++) { // probably a more effiecient 
         inputCheck[i] = toupper(inputCheck[i]);      // way to do this but it works
      }

      if (inputCheck.compare(exitString) == 0) { // check exit case
         return;
      }

      // validate input string
      for (string::reverse_iterator rit = inputCheck.rbegin(); rit != inputCheck.rend(); rit++) { // iterator practice
         if (*rit == '1' || *rit == '0') {
            binVec.push_back(*rit);
         }
         else { // clear vector, clear input, set flag, error message
            cerr << "Invalid input: binary is 0 or 1 only" << endl;
            validInput = false;
            binVec.clear();
            cin.clear();
         }
      }

      if(validInput != true){  // check valid input
         continue;
      }
      else{ // input is valid, convert to dec using power of 2
         for (int i = 0; i < binVec.size(); i++){
            total += (binVec[i] - '0') * pow(2,i);
         }
      }

      if (total < 0){ //Overflow handler
         cerr << "Overflow error max 32-bit signed binary int exceeded" << endl;
         continue;
      }

      cout << total << endl;

   } while (binToDecExit != true);
} // end binToDec

// Converts hexadecimal number to a binary number representation in a do while loop until exit
// String input is verified before proceeding
// Only Hexadecimal digits accepted: 0-9 and A-F
// Automatically capitalizes input so any valid letter will be converted includes exit case
// leading 0's are removed
// upper bound of 8 characters
// @return nothing - results printed to console
void hexToBin(){
   //variable declarations
   vector<char> hexVec; // maybe change to dynamic to allow mutiple instances
   bool validInput = true;
   bool hexToBinExit = false; // exit flag
   string exitString = "EXIT";
   string inputCheck = "";
   string total = "";

   do {
      // reset variables to allow multiple loops
      validInput = true; // needed to reset flag to prevent forever false being set
      total = ""; // reset total
      hexVec.clear(); // clear

      cout << "\n" << "input 8 digit Hexadecimal number or \"EXIT\" to return to main menu: ";
      getline(cin, inputCheck);

      if (inputCheck.length() < 1){ // check empty string
         cerr << "no input detected" << endl;
         continue;
      }

      if (inputCheck.length() > 8){ // probably not needed, but keeping it for consistency 
         cerr << "8 bits (a byte) hexadecimal number supported" << endl;
         continue;
      }

      // force uppercase for easier string parsing
      for (int i = 0; i < inputCheck.length(); i++) { // probably a more effiecient 
         inputCheck[i] = toupper(inputCheck[i]);      // way to do this but it works
      }

      if (inputCheck.compare(exitString) == 0) { // check exit case
         return;
      }

      // validate input string
      for (string::reverse_iterator rit = inputCheck.rbegin(); rit != inputCheck.rend(); rit++) { // iterator practice
         if ((*rit >= '0' && *rit <= '9') || (*rit >= 'A' && *rit <= 'F')) {
            hexVec.push_back(*rit);
         }
         else if (*rit == 'X') { // cases where 0x was added front of hexdecimal number, just parses it off
            break;
         }
         else { // error message, clear vector, clear input, set flag to false
            cerr << "Invalid input: hexdecimal is 0-9 and A-F" << endl;
            validInput = false;
            hexVec.clear();
            cin.clear();
            break;
         }
      }

      if (validInput != true){
         continue;
      }
      else{ //here only 0-9 and A-F is possible
         for (int i = hexVec.size()-1; i >= 0; i--){
            if (hexVec[i] >= 48 && hexVec[i] <= 57){
               total += BIN_STR[hexVec[i] - '0']; // 0-9 indexes
            }
            else{
               total += BIN_STR[hexVec[i] - '7']; // make this eqaul to 10-15
            }
         }
         
         /* hard bouned by 8 digits so this shouldn't be an issue, gonna leave in just in case
            I change my mind in the future - honestly not sure if overflow is a concern

         if(total < 0){ // best solution I could come up with (research better solutions)
            cerr << "Overflow Error max 32 bit signed binary int is 7FFFFFFF" << endl;
            continue;
         } */

         string::iterator it = total.begin();
         while(*it != '1' && it != total.end()-1){
            total.erase(it);
         }
                 
         cout << total << endl;
         
      }
   } while (hexToBinExit != true);
} // end hexToBin

// Conversts binary number to it's 32 bit int decimal eqivalent in a loop until exited
// String input is verified before proceeding
// Only Binary digits accepted 1's and 0's - Exit case checked first
// Automatically capitalizes input so any valid letter will be converted includes exit case
// Overflow handler - based on 32 bit int.
// @return nothing - results printed to console
void binToHex(){
   vector<char> binVec; // maybe change to dynamic to allow mutiple instances
   bool validInput = true;
   bool binToHexExit = false; // exit flag
   string exitString = "EXIT";
   string inputCheck = "";
   stack<char> binStack; // practice with stacks
   string total = "";

   do {
      // reset variables to allow multiple loops
      validInput = true; // needed to reset flag to prevent forever false being set
      total = ""; // reset total
      binVec.clear(); // clear
      cout << "\n" << "input Binary number or \"EXIT\" to return to main menu: ";
      getline(cin, inputCheck);

      if (inputCheck.length() < 1){ // check empty string
         cerr << "no input detected" << endl;
         continue;
      }

      // force uppercase for easier string parsing
      for (int i = 0; i < inputCheck.length(); i++) { // probably a more effiecient 
         inputCheck[i] = toupper(inputCheck[i]);      // way to do this but it works
      }

      if (inputCheck.compare(exitString) == 0) { // check exit case
         return;
      }

      // adds leading zeros to ensure 4 bit nibbles are satisfied.
      while(inputCheck.length() % 4 != 0){
         inputCheck.insert(0, "0");
      }

      // validate input string
      for (string::reverse_iterator rit = inputCheck.rbegin(); rit != inputCheck.rend(); rit++) { // iterator practice
         if (*rit == '1' || *rit == '0') {
            binVec.push_back(*rit);
         }
         else { // clear vector, clear input, set flag, error message
            cerr << "Invalid input: binary is 0 or 1 only" << endl;
            validInput = false;
            binVec.clear();
            cin.clear();
         }
      }

      if(validInput != true){  // check valid input
         continue;
      }
      else{ // input is valid, get 4 bit nibbles, math to HEX_CHARS, then concat chars.
         for (int i = binVec.size() - 1; i >= 0; i--){
            binStack.push(binVec[i]);
            if (binStack.size() == 4){
               total += getHexChar(binStack);
               while (!binStack.empty()){
                  binStack.pop();
               }
            }
         }
      }

      /* I don't think this is needed - leaving in just in case
      if (total < 0){ //Overflow handler
         cerr << "Overflow error max 32-bit signed binary int exceeded" << endl;
         continue;
      }*/

      cout << total << endl;

   } while (binToHexExit != true);

}// end binToHex

/* due to time constraints and money constraints these methods will be implemented at a later date

void decToHex() {
   // variable declarations
   bool decToHexExit = false; // exit flag
   string exitString = "EXIT";
   string inputCheck = "";

   do {
      cout << "\n" << "input Decimal number or \"EXIT\" to return to main menu: ";
      getline(cin, inputCheck);

      // force uppercase for easier string parsing
      for (int i = 0; i < inputCheck.length(); i++) { // probably a more effiecient 
         inputCheck[i] = toupper(inputCheck[i]);      // way to do this but it works
      }

      if (inputCheck.compare(exitString) == 0) { // check exit case
         return;
      }
      cout << inputCheck << endl;
   } while (decToHexExit != true);
}

void decToBin() {
   //variable declarations
   bool decToBinExit = false; // exit flag
   string exitString = "EXIT";
   string inputCheck = "";

   do {
      cout << "\n" << "input Decimal number or \"EXIT\" to return to main menu: ";
      getline(cin, inputCheck);

      // force uppercase for easier string parsing
      for (int i = 0; i < inputCheck.length(); i++) { // probably a more effiecient 
         inputCheck[i] = toupper(inputCheck[i]);      // way to do this but it works
      }

      if (inputCheck.compare(exitString) == 0) { // check exit case
         return;
      }
      cout << inputCheck << endl;
   } while (decToBinExit != true);
}
*/
