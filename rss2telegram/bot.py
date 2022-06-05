#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import telebot
import logging

from telebot.types import InlineKeyboardMarkup, InlineKeyboardButton
from utils import Database, Subscription, SubscriptionType, UserState
from grabber import Grabber

users_states = {}

# https://github.com/eternnoir/pyTelegramBotAPI
bot = telebot.TeleBot(os.environ['TELEGRAM_TOKEN'], parse_mode=None)
db_session = Database().get_session()

@bot.message_handler(commands=['subscribe'])
def process_command_subscribe(message):
    """Process SUBSCRIBE command"""
    logging.debug('Subscribe: %d', message.from_user.id)
    users_states[message.from_user.id] = UserState.SUBSCRIBE
    bot.reply_to(message, 'Enter URL to subscribe')

@bot.message_handler(commands=['unsubscribe'])
def process_command_unsubscribe(message):
    """Process UNSUBSCRIBE command"""
    logging.debug('Unsubscribe: %d', message.from_user.id)
    users_states[message.from_user.id] = UserState.UNSUBSCRIBE
    subscriptions = db_session.query(Subscription).filter(Subscription.user_id == message.from_user.id)
    bot.reply_to(message, 'Here are your subscriptions. From which unsubscribe to?')
    for subscription in subscriptions:
        reply = '{0}\n{1}'.format(subscription.title, subscription.url)
        bot.reply_to(message, reply)

@bot.message_handler(func=lambda message: True)
def process_command_text(message):
    user_state = users_states.get(message.from_user.id, UserState.IDLE)
    reply = "What?"

    """Handle rest messages"""
    if user_state == UserState.SUBSCRIBE:
        """Handle URL of new subscription"""
        subscription = db_session.query(Subscription).filter(Subscription.user_id == message.from_user.id).filter(Subscription.url == message.text).first()
        if subscription:
            reply = 'You already subscribed'
        else:
            subscription = Subscription(message.from_user.id, message.text, SubscriptionType.RSS, '', None)
            #TODO process various subscription types (RSS, ...)
            db_session.add(subscription)
            db_session.commit()
            reply = 'ok'

    elif user_state == UserState.UNSUBSCRIBE:
        """Handle forwarded URL to unsubscribe from"""
        #TODO check by name or url
        subscription = db_session.query(Subscription).filter(Subscription.user_id == message.from_user.id).filter(Subscription.url == message.text).first()
        if subscription:
            db_session.delete(subscription)
            db_session.commit()
            reply = 'ok'

    else:
        """Handle other messages"""
        pass
    users_states[message.from_user.id] = UserState.IDLE
    bot.reply_to(message, reply)

logging.basicConfig(level=logging.INFO)

grabber = Grabber()
grabber.start(bot, 2)

bot.delete_my_commands(scope=None, language_code=None)
bot.set_my_commands(
    commands=[
        telebot.types.BotCommand('subscribe', 'subscribe to feed'),
        telebot.types.BotCommand('unsubscribe', 'unsubscribe from feed'),
    ],
)
bot.infinity_polling()
