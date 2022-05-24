import os
import telebot
from telebot.types import InlineKeyboardMarkup, InlineKeyboardButton
from database import Database, Task, UserState

users_states = {}

bot = telebot.TeleBot(os.environ['TELEGRAM_TOKEN'], parse_mode=None)
db_session = Database().get_session()

@bot.message_handler(commands=['add'])
def process_command_add(message):
    """Process ADD command"""
    users_states[message.from_user.id] = UserState.CREATE
    bot.reply_to(message, 'Enter task text:')

@bot.message_handler(commands=['del'])
def process_command_remove(message):
    """Process DELETE command"""
    users_states[message.from_user.id] = UserState.DELETE
    bot.reply_to(message, 'Which remove? Forward it here.')

@bot.message_handler(commands=['list'])
def process_command_list(message):
    """Process LIST command"""
    users_states[message.from_user.id] = UserState.IDLE
    tasks = db_session.query(Task).filter(Task.user_id == message.from_user.id)
    for task in tasks:
        bot.reply_to(message, task.text)

@bot.message_handler(func=lambda message: True)
def echo_all(message):
    reply = 'What?'
    user_state = users_states.get(message.from_user.id, UserState.IDLE)
    """Handle rest messages"""
    if user_state == UserState.CREATE:
        """Handle text of new task"""
        task = Task(message.from_user.id, message.text)
        db_session.add(task)
        db_session.commit()
        reply = 'ok'
        users_states[message.from_user.id] = UserState.IDLE

    elif user_state == UserState.DELETE:
        """Handle forwarded task to remove"""
        task = db_session.query(Task).filter(Task.user_id == message.from_user.id).filter(Task.text == message.text).first()
        if task:
            db_session.delete(task)
            db_session.commit()
            reply = 'ok'
            users_states[message.from_user.id] = UserState.IDLE

    else:
        """Handle other messages"""
        pass

    bot.reply_to(message, reply)

bot.delete_my_commands(scope=None, language_code=None)
bot.set_my_commands(
    commands=[
        telebot.types.BotCommand("add", "add task"),
        telebot.types.BotCommand("del", "delete task"),
        telebot.types.BotCommand("list", "list tasks"),
    ],
)
bot.infinity_polling()
