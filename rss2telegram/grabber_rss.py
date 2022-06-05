#!/usr/bin/env python
# -*- coding: utf-8 -*-

import feedparser
from datetime import datetime
from time import mktime

from grabberinterface import GrabberInterface
from utils import Item

import logging

class GrabberRss(GrabberInterface):
    """RSS feed grabber."""
    def process(self, subscription):
        data = feedparser.parse(subscription.url)
        if not 'feed' in data:
            return None

        """check subscription title"""
        if subscription.title == '':
            subscription.title = data['feed']['title']

        """check last update datetime"""
        dt = datetime.fromtimestamp(mktime(data['feed']['published_parsed']))
        if subscription.lastupdate != None and dt < subscription.lastupdate:
            return None

        items = []
        for item in data['entries']:
            item_dt = datetime.fromtimestamp(mktime(item['published_parsed']))
            if subscription.lastupdate != None and item_dt < subscription.lastupdate:
                continue
            item = Item(item['link'], item['title'], item['description'])
            items.append(item)
        if (len(items)) > 0:
            subscription.lastupdate = dt;

        return items

        
