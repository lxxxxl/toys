#!/usr/bin/env python
# -*- coding: utf-8 -*-

import logging
from threading import Timer
from grabber_rss import GrabberRss
from utils import Database, Item, Subscription, SubscriptionType


class Grabber():
    """Performs grabbing from all subscriptions"""
    grabber_rss = GrabberRss()
    db_session = Database().get_session()

    def start(self, bot, interval):
        """Starts grabbing of subscriptions.

        Keyword arguments:
        bot -- telebot.TeleBot instanse to send replies
        interval -- grabbing interval in hours

        """
        self.bot = bot
        self.interval = interval

        self.grab_timer_start(self.interval)

    def grab_timer_start(self, hours):
        """Execute grab Subscriptions procedure every X hours"""
        timer = Timer(60 * 60 * hours, self.grab)
        timer.start()

    def grab(self):
        """grab subscriptions"""
        subscriptions = self.db_session.query(Subscription).all()
        for subscription in subscriptions:
            if (subscription.type == SubscriptionType.RSS):
                items = self.grabber_rss.process(subscription)
                for item in items:
                    message = item.url
                    self.bot.send_message(subscription.user_id, item.url)
                    #logging.info('%d %r',subscription.user_id, item.url)
                    """No need to send title and description because Telegram manually creates URL previews"""
                
        self.db_session.commit()

        self.grab_timer_start(self.interval)



