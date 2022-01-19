####################### imports ######################
from typing import Final
from aiohttp.helpers import noop

from pynput import keyboard
from pynput.keyboard import Key, Controller
from pynput.mouse import Button
from pynput.mouse import Controller as Cont
from pynput.keyboard import Listener

import win32api, win32con

from twitchio.ext import commands
from twitchio.client import Client

from imap_tools import MailBox, A

import csv
import os
from threading import *
import time

####################### enviroment variables ######################
username = os.environ['EMAIL_USERNAME']
password = os.environ['EMAIL_PASSWORD']
ttv_token = os.environ['TWITCH_TOKEN']
ttv_client = os.environ['TWITCH_CLIENT_ID']
ttv_secret = os.environ['TWITCH_CLIENT_SECRET']

####################### file names ######################
csvFileName = "C:\\Users\\Colton\\Source\\Repos\\KoFeTest\\UserTokens.csv"
variableTextFileName = "C:\\Users\\Colton\\Source\\Repos\\KoFeTest\\variableText.txt"
commandsActiveTextFileName = "C:\\Users\\Colton\\Source\\Repos\\KoFeTest\\commandsActive.txt"



####################### Objects ######################
keyboard = Controller()
mouse = Cont()

bot = commands.Bot(token=ttv_token, client_secret=ttv_secret, prefix='!', initial_channels=['horsesNhalo'])
client = Client( token=ttv_token, client_secret=ttv_secret)

####################### Globals ######################
botActive = True #determines if bot commands will work or not
shifted = False #tells if the shift key is currently held
drunkActive = False
drunkTime = 0
timedEvent = None

####################### Constants ######################
baseTokens: Final = 10 #the number of tokens new user to the channel recieves


####################### class definitions ######################
class TimedEvent(Thread):
    def run(self):
        global drunkActive
        keyboard.press(Key.f21)
        keyboard.release(Key.f21)
        print("start")
        time.sleep(drunkTime)
        keyboard.press(Key.f21)
        keyboard.release(Key.f21)
        drunkActive = False
        print("done")

####################### Token Functions ######################

def GetTokens(twitchName) -> int:
    tokenDict = {}
    with open(csvFileName, mode='r') as infile: #load in dictionary from file
        reader = csv.reader(infile)
        tokenDict = {rows[0]:rows[1] for rows in reader}
    
    with MailBox('imap.gmail.com').login(username, password, 'INBOX') as mailbox: #Read in emails from mailbox
        for msg in mailbox.fetch(A(all=True)):
            sender = msg.from_
            if sender != "hello@ko-fi.com":
                continue
            body = msg.text or msg.html
            priceStart = body.index("$ ")
            priceEnd = body.index("\n", priceStart)
            tokens = int(float(body[(priceStart + 2) : (priceEnd - 1)]) * baseTokens)

            twitchNameStart = body.index(": ", priceEnd)
            twitchNameEnd = twitchNameStart + 2
            searching = True
            while(searching) :
                if(str.isspace(body[twitchNameEnd])) :
                    searching = False
                else :
                    twitchNameEnd += 1

            emailTwitchName = body[(twitchNameStart + 2) : (twitchNameEnd)].lower()

            value = tokenDict.get(emailTwitchName)
            if(value == None):  #if twitchName is not in dictionary, give it base tokens + the amount they bought.
                tokenDict[emailTwitchName] = baseTokens + tokens
            else : #otherwise just add on the amount they bought
                tokenDict[emailTwitchName] = str(int(tokenDict[emailTwitchName]) + tokens)
            
            mailbox.move(msg.uid, "Processed")
    
    result = baseTokens
    value = tokenDict.get(twitchName)

    if(value == None):  #if twitchName is not in dictionary, give it base tokens
        tokenDict[twitchName] = baseTokens
    else : #otherwise just add on the amount they bought
        result = int(value)
    
    with open(csvFileName, mode='w') as outfile: #write it back into the file
        for user in tokenDict:
            outfile.write(user + "," + str(tokenDict[user]) + "\n")

    return result

#with a prior call to get tokens, this is guarenteed to have twitchName in the dictionary
#the value of tokens can be positive to give tokens, or negative to take tokens.
def UpdateTokens(twitchName, tokens):
    tokenDict = {}
    with open(csvFileName, mode='r') as infile: #load in dictionary from file
        reader = csv.reader(infile)
        tokenDict = {rows[0]:rows[1] for rows in reader}

    value = int(tokenDict[twitchName])
    tokenDict[twitchName] = value + tokens

    with open(csvFileName, mode='w') as outfile: #write it back into the file
        for user in tokenDict:
            outfile.write(user + "," + str(tokenDict[user]) + "\n")

####################### Management Functions ######################

def ToggleBotCommands() :
    global botActive
    botActive = (not botActive)

    with open(commandsActiveTextFileName, mode='w') as outfile: #write it back into the file
        outfile.write("chat commands are: " + ("ON" if botActive else "OFF"))

def on_press(key):
    global shifted
    if(key == Key.shift) :
        shifted = True

def on_release(key):
    global shifted
    if(key == Key.shift) :
        shifted = False
    if(key == Key.f20) :
        if(shifted) : #If command is shifted, it will kill the program
            print("exiting")
            if(botActive) :
                ToggleBotCommands()
            os._exit(0)
        else : #Otherwise just toggle whether it's active or not
            ToggleBotCommands()

####################### Bot Commands ######################

@bot.event #called on every message
async def event_message(ctx):
    print(ctx.author)
    print(ctx.content)
    await bot.handle_commands(ctx)

@bot.event #called when bot connects
async def event_ready():
    print("Bot Online!")


@bot.command(name='test')
async def command_test(ctx):
    await ctx.send("You successfully input a command! Yay! The Test worked!")

@bot.command(name='help')
async def command_help(ctx):
    await ctx.send("""
    Try some commands out!
    View list of commands on ko-fi.com/horsesNhalo. 
    I'd post them here but twitch doesn't let me format 
    my messages so it looks ugly. You can also buy 
    command tokens on ko-fi to help support (and influence) 
    my stream. You start with 10 free tokens.
    """)

commandCosts = {
    'get_tokens': 0,
    'clicky': 1,
    'space': 1,
    'melee': 2,
    'set_filter': 2,
    'move_mouse': 5,
    'custom_text':10,
    'drunk' : 10,
    'alt_tab': 50,
    'alt_f4': 100,
}

@bot.command(name='get_tokens')
async def command_get_tokens(ctx):
    tokens = int(GetTokens(ctx.author.name))
    await ctx.send('you have ' + str(tokens) + ' tokens')

@bot.command(name='clicky')
async def command_clicky(ctx):
    if(not botActive) : 
        await ctx.send("sorry commands are offline at the moment, please write a formal complaint in chat... which I will ignore.")
    else :
        tokens = int(GetTokens(ctx.author.name))
        needed = commandCosts['clicky']
        if(tokens < needed) :
            await ctx.send('You only have  ' + str(tokens) + ' tokens, you need at least  ' + str(needed) + '  tokens for this command.')
        else : 
            mouse.press(Button.left)
            mouse.release(Button.left)
            await ctx.send('left mouse button clicked')
            UpdateTokens(ctx.author.name, -needed)
    
@bot.command(name='space')
async def command_alt_f4(ctx):
    if(not botActive) : 
        await ctx.send("sorry commands are offline at the moment, please write a formal complaint in chat... which I will ignore.")
    else :
        tokens = int(GetTokens(ctx.author.name))
        needed = commandCosts['space']
        if(tokens < needed) :
            await ctx.send('You only have  ' + str(tokens) + ' tokens, you need at least  ' + str(needed) + '  tokens for this command.')
        else : 
            keyboard.press(Key.space)
            keyboard.release(Key.space)
            UpdateTokens(ctx.author.name, -needed)
            await ctx.send('You have successfully pressed the space bar')
            
    
@bot.command(name='melee')
async def command_melee(ctx):
    if(not botActive) : 
        await ctx.send("sorry commands are offline at the moment, please write a formal complaint in chat... which I will ignore.")
    else :
        tokens = int(GetTokens(ctx.author.name))
        needed = commandCosts['melee']
        if(tokens < needed) :
            await ctx.send('You only have  ' + str(tokens) + ' tokens, you need at least  ' + str(needed) + '  tokens for this command.')
        else : 
            keyboard.press('v')
            keyboard.release('v')
            UpdateTokens(ctx.author.name, -needed) 
            await ctx.send('You have successfully made me go punchy')


oldFilter = 0
fkeys = [Key.f13, Key.f14, Key.f15, Key.f16, Key.f17, Key.f18, Key.f19]
filterKeyCount = 7
filterNames = {
    'chief': 0,
    'alien': 1,
    'fire': 2,
    'chicken': 3,
    'octane': 4,
    'horse':5,
    'girl': 6,

    'cat': 7,
    'sexy': 8,
    'demon': 9,
    'smile': 10,
    'king': 11,
    'eye': 12,
    'baby': 13,
}
@bot.command(name='set_filter')
async def command_set_filter(ctx, arg):
    # if(not botActive) : 
    #     await ctx.send("sorry commands are offline at the moment, please write a formal complaint in chat... which I will ignore.")
    # else :
    tokens = int(GetTokens(ctx.author.name))
    needed = commandCosts['set_filter']
    if(tokens < needed) :
        await ctx.send('You only have  ' + str(tokens) + ' tokens, you need at least  ' + str(needed) + '  tokens for this command.')
    else : 
        try :
            global oldFilter
            currentFilter = int(filterNames[arg])
            if(currentFilter == oldFilter) :
                await ctx.send("sorry, already showing the " + arg + " filter, try a differnt one")
            else :
                if(currentFilter >= filterKeyCount) : # if the index is greater than array, circle back with a different combo key
                    with keyboard.pressed(Key.shift):
                        keyboard.press(fkeys[currentFilter - 7])
                        keyboard.release(fkeys[currentFilter - 7])
                else :
                    with keyboard.pressed(Key.alt):
                        keyboard.press(fkeys[currentFilter])
                        keyboard.release(fkeys[currentFilter])
                await ctx.send('Now showing the ' + arg + ' filter.')
                oldFilter = currentFilter
                UpdateTokens(ctx.author.name, -needed)
        except :
            await ctx.send(arg + " is not a valid filter. Please look at filters on command list on ko-fi.com/horsesnhalo")

posCount = 4
currentPos = 0
positions = [1000, -1000]
@bot.command(name='move_mouse')
async def command_move_mouse(ctx):
    if(not botActive) : 
        await ctx.send("sorry commands are offline at the moment, please write a formal complaint in chat... which I will ignore.")
    else :
        tokens = int(GetTokens(ctx.author.name))
        needed = commandCosts['move_mouse']
        if(tokens < needed) :
            await ctx.send('You only have  ' + str(tokens) + ' tokens, you need at least  ' + str(needed) + '  tokens for this command.')
        else : 
            global currentPos
            currentPos = (currentPos + 1) % posCount
            x = positions[int(currentPos/2)]
            y = positions[int(currentPos%2)]
            # mouse.move(x,y) this didn't workin in game, but the following line did:
            win32api.mouse_event(win32con.MOUSEEVENTF_MOVE, x, y, 0, 0)
            await ctx.send('Mouse has been moved horizontally ' + str(x) + ' units and vertically ' + str(y) + ' units.')
            UpdateTokens(ctx.author.name, -needed)
            
@bot.command(name='custom_text')
async def command_custom_text(ctx, arg):
    # if(not botActive) : 
    #     await ctx.send("sorry commands are offline at the moment, please write a formal complaint in chat... which I will ignore.")
    # else :
    tokens = int(GetTokens(ctx.author.name))
    needed = commandCosts['custom_text']
    if(tokens < needed) :
        await ctx.send('You only have  ' + str(tokens) + ' tokens, you need at least  ' + str(needed) + '  tokens for this command.')
    else : 
        upperbound = 50
        input = arg
        if(len(input) > upperbound) :
            await ctx.send("try again with text less than " + str(upperbound) + " characters")
        else :
            with open(variableTextFileName, mode='w') as outfile: #write it back into the file
                outfile.write("Chat decided this message was important: " + input)
            await ctx.send('your text has been set and will stay until another user changes it')
            UpdateTokens(ctx.author.name, -needed)
            
@bot.command(name='drunk')
async def command_drunk(ctx, arg=10):
    global drunkActive
    global drunkTime
    global timedEvent
    if(not botActive) : 
        await ctx.send("sorry commands are offline at the moment, please write a formal complaint in chat... which I will ignore.")
    elif (drunkActive) :
        await ctx.send("sorry, I'm already drunk (סּںסּَ`)")
    else :
        tokens = int(GetTokens(ctx.author.name))
        needed = commandCosts['drunk']
        if(arg != None) :
            try : 
                amount = int(arg)
                if(amount >= needed) : 
                    needed = amount
                else :
                    await ctx.send(str(amount) + " is not enough tokens for the command, try using 10 or more.")
                    return
            except :
                noop
        if(tokens < needed) :
            await ctx.send('You only have  ' + str(tokens) + ' tokens, you need at least  ' + str(needed) + '  tokens for this command.')
        else : 
            drunkActive = True
            drunkTime = needed
            timedEvent = TimedEvent()
            timedEvent.start()
            await ctx.send('Now showing the drunk effect for ' + str(needed) + ' seconds.')

            UpdateTokens(ctx.author.name, -needed)

@bot.command(name='alt_tab')
async def command_alt_tab(ctx):
    if(not botActive) : 
        await ctx.send("sorry commands are offline at the moment, please write a formal complaint in chat... which I will ignore.")
    else :
        tokens = int(GetTokens(ctx.author.name))
        needed = commandCosts['alt_tab']
        if(tokens < needed) :
            await ctx.send('You only have  ' + str(tokens) + ' tokens, you need at least  ' + str(needed) + '  tokens for this command.')
        else : 
            with keyboard.pressed(Key.alt):
                keyboard.press(Key.tab)
                keyboard.release(Key.tab)
            await ctx.send('alt tab command has been sent')
            UpdateTokens(ctx.author.name, -needed)

@bot.command(name='alt_f4')
async def command_alt_f4(ctx):
    if(not botActive) : 
        await ctx.send("sorry commands are offline at the moment, please write a formal complaint in chat... which I will ignore.")
    else :
        tokens = int(GetTokens(ctx.author.name))
        needed = commandCosts['alt_f4']
        if(tokens < needed) :
            await ctx.send('You only have  ' + str(tokens) + ' tokens, you need at least  ' + str(needed) + '  tokens for this command.')
        else : 
            with keyboard.pressed(Key.alt):
                keyboard.press(Key.f4)
                keyboard.release(Key.f4)
            await ctx.send('alt f4 command has been sent')
            UpdateTokens(ctx.author.name, -needed)

# @bot.command(name='who')
# async def get_chatters(ctx):
#     chatters = await client.get_chatters('horsesNhalo')
#     all_chatters = ' '.join(chatters.all)
#     await ctx.send(f"In chat: {all_chatters}")

####################### Main ######################

if __name__ == '__main__':
    listener = Listener(on_press=on_press, on_release=on_release)
    listener.start()
    print("booted up")
    ToggleBotCommands() #turn off bot commands on startup. I can start them when I start gaming
    bot.run() #blocking command, program is now up and running