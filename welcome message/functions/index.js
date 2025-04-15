/**
 * Import function triggers from their respective submodules:
 *
 * const {onCall} = require("firebase-functions/v2/https");
 * const {onDocumentWritten} = require("firebase-functions/v2/firestore");
 *
 * See a full list of supported triggers at https://firebase.google.com/docs/functions
 */
const functions = require('firebase-functions/v1');
const admin = require("firebase-admin");
admin.initializeApp();

const {onRequest} = require("firebase-functions/v2/https");
const logger = require("firebase-functions/logger");

// Create and deploy your first functions
// https://firebase.google.com/docs/functions/get-started

exports.helloWorld = onRequest((request, response) => {
  logger.info("Hello logs!", {structuredData: true});
  response.send("Hello from Firebase!");
});

exports.checkFirstTimeUser = functions.https.onCall(async (request) => {
  // Check if user is authenticated
  if (!request.auth) {
    throw new functions.https.HttpsError(
      'unauthenticated',
      'User must be authenticated to check first time status'
    );
  }

  const userId = request.auth.uid;
  const userRef = admin.database().ref(`users/${userId}`);

  try {
    // Get user data
    const snapshot = await userRef.once('value');
    const userData = snapshot.val();

    // If user data doesn't exist or hasVisitedWelcome is false, it's their first time
    const isFirstTime = !userData || !userData.hasVisitedWelcome;

    if (isFirstTime) {
      // Update the user's record to mark welcome as visited
      await userRef.update({
        hasVisitedWelcome: true,
        lastWelcomeVisit: admin.database.ServerValue.TIMESTAMP,
        firstVisitDate: admin.database.ServerValue.TIMESTAMP
      });
    }

    return {
      isFirstTime,
      message: isFirstTime ? 'Welcome to ProPlanner!' : 'Welcome back to ProPlanner!'
    };
  } catch (error) {
    console.error('Error checking first time user:', error);
    throw new functions.https.HttpsError(
      'internal',
      'Error checking first time user status'
    );
  }
});
