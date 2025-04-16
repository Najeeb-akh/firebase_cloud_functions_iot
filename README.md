# Firebase Cloud Functions

tutorial Link: https://technion.zoom.us/clips/share/HbrKCtcLQkmqXMn7olNqUQ


![image](https://github.com/user-attachments/assets/4f13fcd6-3214-4118-8617-5f40a1ef1c6b)


Used commands and instructions: 

Firebase setup: 
	- 	go to firebase console home page 
	⁃	create a new firebase project if you haven’t already 
	⁃	after you have setup your app we move to the next step: select the build section in the left side banner and click on functions

- you might need to contact the course staff to provide a payment method to unlock this feature.

- now that we have setup the firebase function feature on the firebase console we now need to deploy the functions from our computer into the firebase. For that we need to move to the next step: 
- firebase cli setup: 
    -> windows users: run these commands: 
					npm install -g firebase-tools
					firebase login
	-> Mac users: 
			- install node.js and npm using homebrew: 
				brew install node
			- verify installation:
				node —version
				npm —version
			- install firebase cli tools:
				npm install -g firebase-tools
				* or add sudo in the beginning for any permission errors
￼
	——————————————————————————————————————————————

- Now that we downloaded the firebase cli tool, we need to navigate to our project folder (this is the folder that you saved all of your project related files- could be anywhere)
 		inside a terminal or cmd: 
				- cd /path/to/ptoject
		
		* note: try navigating to a local directory that is saved directly to your pc/laptop - no oneDrive, iCloud or any cloud maintained folders/paths.

- Authentication : this is the step that will finally connect the project that you have setup on the firebase console to your project folder on your pc. use the following command: 
				- firebase login
		* if you have accidentally signed in a different google account in the popup web page you can easily write “ firebase logout “ and repeat the process.

- Initialize firebase functions: 
		- firebase init functions
		-> this will create a functions folder alongside all the required files needed for the cloud functions service, this 	will prompt you to choose the language of the cloud functions (javascript, typescript or python), choose your 	preferable language in my tutorial I’ve chosen javascript. Also choose to install dependencies when prompted.
 
- Add or edit your cloud functions: 
		- open the “ index.js “ (or index.ts / index.py) in your preferable IDE
		- insert your function code inside the file ( the functions in our tutorial can be found on the GitHub Link provided)
		- save the file

- 	Deployment: 
		- firebase deploy —only functions
		this will deploy our functions to the cloud functions service in the firebase, now if we head back the firebase page, under the cloud functions we can see the newly added function that we have deployed: 
			￼

		* if the waiting for new deployment message is still appearing on the screen , make sure that the index file was saved before deployment, and that you are signed in and connected to the correct app from the firebase cli, and that the functions folder is inside the directory that we initialized the firebase in.
