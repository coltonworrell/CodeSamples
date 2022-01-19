So this is just a rough compilation of some of the work I've done throughout my college career. This file serves as a brief
overview of each project/sample. Much of the code is suboptimal to what I would like to share, and contains bits of magic
numbers, humor, and documents my struggles more than I'd care to reveal. But I come from a culture where honesty is far 
better than overselling yourself, so if ensuring you get an accurate depiction of my abilities AND flaws means feeling a
little embarrassed it's worth it.

* DigiPengine.zip - This project is one that started as my sophomore engine, then I evolved it again and again each semester.
	The main files to focus on in this are the various generators, which are tools that allow faster implementation time
	for new pieces of the engine, which are primarily generalized as systems, components, and events. The other file
	to pay attention to is Properties.h. In this file I attempt to have a form of pseudo runtime type reflection that
	can scale reasonably well, so that I can automate tasks of serialization, exposing to a map editor, and recording
	changes of state. It was a lot of overhead, which when combined with trying to use BGFX for graphics, killed the project
	when I ran into linker errors. This version of the engine is relatively stripped to its core. I hope to return to it one
	day and make something I can actually demonstrate rather than just theorize about. My driving goals for this engine was
	to run a script to generate a new component, event, or system, and it would instantly be able to be saved, loaded, 
	modified in an editor, included in the project, and simply uncommenting certain lines would yield a working example 
	variable. There would be no need to write ANY code in any other files aside from files the user is adding. Simply 
	enabling a single macro would enable every change of state within the game to be recorded, allowing a rewind mechanic, 
	or advanced debug mode. I had proof of concepts for each of these pieces, but it's become a good lesson in the 
	importance of version control because all have since been lost.

 * Editor.cpp - This is from another version of DigiPengine (also broken, but I opted out of sharing it as some graphics
	code was by another student on the same project). This was the map editor for the game, which used a mixture of my
	property system, along with IMGUI to demonstrate how easily a map editor can be made if variable were wrapped in 
	Properties (my pseudo runtime type reflection).

 * esp32test.cpp - This testing app I wrote was for my ESP32 AI Thinker Cam module. I was testing to see if I could get QR
	detection and a servo motor running at the same time for a problem I was trying to solve. I was able to, but only after
	doing a lot of research. It turns out that many general purpose input/output pins were being used by the QR detection
	code, so the pins I could use for the servo were limited vastly. Working with microcomputers has shown me how severely
	underdocumented and particular these devices can be.

 * Math366.cpp - This was an interesting project. I improved on this code during each lecture of my combinatorics class.
	I was attempting to convert every mathematical formula to c++. I was able to document each function we learned in class,
	and take notes within this file. Much of what is commented out was questions we were asked to solve in class, or
	elements of those questions. I found this to be a very helpful tool for me to learn these concepts, as I find it more
	comfortable to read these formulas as c++ code than standard mathematical formulas.

 * VRScreenshot.png - This is more or less just a screenshot of the starter project for VR in UE4. Regardless, it helped me
	to learn getting started with a VR project and how to set the default settings. I also change an element of the walkable
	area to include the walls so I could hop up on them. I then compiled the project and uploaded it to my Oculus with adb.
	My goal is to create my vision for my dream house and property within VR when I find the time, but as of now this project
	just serves to show that I have indeed at least tampered with VR.

 * Queen.cs - This file was for a final project in my math class on combinatorial game theory. The
	goal was to create a digital version of the board game Amazons, and then make AI for that game. This file contains
	behavior for that AI, which may be either random, or to play optimally for as far as it can see ahead in the game (a 
	user defined set number of moves). This code was more rushed so it has less explanation for variable names than I would
	like. Looking back now it definitely reinforces the need to have well documented code.

 * TextTriggerBox.uasset - This is one of the blueprints I worked on in my Junior/Senior game team for our project D-BUG. 
	The code is a simple trigger to have text display when you run into the invisible actor it is attached to. It is then 
	displayed for the set amount of time on the character's HUD. Some of these boxes were only meant to be displayed a single
	time, these were destroyed instantly after they set the text to display. Others would reset and show their given text 
	each time the player passed through them. This file was written later in the project, and I had the luxury of creating a
	new blueprint for it. The other blueprints within the project were horrendously bloated, and I learned a lot about how to
	properly separate blueprint code out after the fact (unfortunately too late in the project to pivot our implementation).

 * The Brilliance of Resilience.zip - This Unity project was a 2nd generation prototype of a brainchild of a mobile game 
	I've had for some time now. I'm currently working on the 3rd generation of this game and hope to polish it enough to
	post it to the Android app store. Making this game has shown me the importance of simple and intuitive input, as I
	had trouble trying to communicate to testers that holding on the screen increases bounciness without using any text. I
	also learned about the importance of centralized UI, as the testers eyes were drawn to the center of the player and
	wouldn't directly look at any of the alerts of danger that were placed within the corners of the screen.

 * twitchbot.py - This game was a proof of concept for another way to make money off of Twitch. I wanted to create some kind
	of currency that users could buy from me, then use to mess with my gameplay. I documented allocations of this currency
	in a CSV file, where 1 token represented 10 cents. A user could buy tokens from my donation page, which would send a
	notification email of how much they spent. I then parsed that email for how many tokens to give the user in my CSV.
	Any time a command came in on twitch my bot would check if they had enough tokens to their username. If they weren't in
	the CSV file, it defaulted to give them 10 tokens to try out some commands. Most commands just activated key presses for
	macros, that would cause different effects on different apps. I used Snap Camera for face filters, Voicemod for voice
	filters, OnTopReplica to create a sort of drunk effect, and a few other commands. My favorite is that for 100 tokens
	(aka $10.00) you could send an alt-f4 command to shut off my game. This was my first real python script that was more
	than just a few lines, and was a lot of fun to write. I learned a lot about using libraries for these web apps and
	writing a bot. I also learned that it's hard to get Twitch viewers when your content depends on having Twitch viewers,
	certainly a cart before the horse problem.

 * WireSorter.png - This is a snippet of code again from our D-BUG project. I did some simple math to find our wire minigame
	had 3^2 possibilities. The wire connected a vertical port to horizontal port, and there was three horizontal and three
	vertical ports. The game would generate a number between 0 and 8 inclusively to represent which configuration the user 
	had to set the wire to. I then had to find what nth configuration the user had switched to by moving the vertical wire 
	up or down, or horizontal wire left or right. It was fun finding the solution on paper and translating it to a blueprint
	code. It was also exciting using the modulo operation for a gameplay mechanic.

Thanks for taking the time to look through these documents. I hope that I may be able to work with you in the future to
enhance your projects and teams, as well as improve upon my own abilities and knowledge.

Regards,
~Colton Worrell
