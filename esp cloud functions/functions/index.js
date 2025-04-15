const {onValueWritten} = require("firebase-functions/v2/database");
const admin = require("firebase-admin");
admin.initializeApp();

/**
 * Convert numbers from 1-100 to words
 * This function listens for changes to 'button_counter' in the realtime database
 * and writes the word form to 'button_counter_word'
 */
exports.numberToWord = onValueWritten('/button_counter', (event) => {
    // Get the current value
    // Fix for the data access - properly extract the value from the snapshot
    const snapshot = event.data.after;
    const number = snapshot.val(); // Use val() method to get the actual data
    
    console.log("Received value:", number);
    
    // If number is deleted or null, exit early
    if (number === null) {
        console.log("Number is null, exiting");
        return null;
    }
    
    // Convert number to integer (in case it's stored as string)
    const num = parseInt(number, 10);
    
    // Check if it's a valid number between 1-100
    if (isNaN(num) || num < 1 || num > 100) {
        console.log("Invalid number:", num);
        return null;
    }
    
    console.log("Converting number:", num);
    
    // Convert the number to words
    const word = convertNumberToWords(num);
    console.log("Converted to word:", word);
    
    // Write the word representation to the database
    return admin.database().ref('/button_counter_word').set(word);
});

/**
 * Helper function to convert a number to its word representation
 * @param {number} num - The number to convert (1-100)
 * @returns {string} The word representation of the number
 */
function convertNumberToWords(num) {
    const ones = ["", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine",
                 "ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen",
                 "seventeen", "eighteen", "nineteen"];
    const tens = ["", "", "twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety"];
    
    if (num === 0) {
        return "zero";
    }
    
    if (num < 20) {
        return ones[num];
    }
    
    if (num < 100) {
        const remainder = num % 10;
        return tens[Math.floor(num / 10)] + (remainder !== 0 ? "-" + ones[remainder] : "");
    }
    
    if (num === 100) {
        return "one hundred";
    }
    
    return "out of range";
}