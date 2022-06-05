#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os

from sqlalchemy import Column, Integer, String, DateTime, ForeignKey, Table, Enum, func
from sqlalchemy.orm import relationship, backref
from sqlalchemy.ext.declarative import declarative_base

from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker, scoped_session

Base = declarative_base()

class Subscription(Base):
    """Info about RSS or other source and owner's user id"""
    __tablename__ = 'subscriptions'
    id = Column(Integer, primary_key=True)
    """Unique id. Fills automatically"""
    user_id = Column(Integer)
    """Owner's user id."""
    url = Column(String)
    """URL with RSS or other feed"""
    type = Column(Integer)
    """Type of subscription, check enum SubscriptionType"""
    title = Column(String)
    """Subscription title"""
    lastupdate = Column(DateTime)
    """Date and time of last update"""
        
    def __init__(self, user_id, url, type, title, lastupdate):
        self.user_id = user_id
        self.url = url
        self.type = type
        self.title = title
        self.lastupdate = lastupdate

    def __repr__(self):
        return "<Subscription('%d','%s')>" % (self.user_id, self.url)

class Database(object):
    """Performs connection to database"""
    dbfilename = 'database.db'
    """Database filename"""
    session = None
    """Database session singleton"""

    def __new__(cls):
        """Singleton constructor"""
        if not hasattr(cls, 'instance'):
            cls.instance = super(Database, cls).__new__(cls)
        return cls.instance

    def get_session(self):
        """Get database session singleton
        session is of type sqlalchemy.orm.session.Session"""
        if self.session != None:
            return self.session
        engine = create_engine('sqlite:///' + self.dbfilename)
        Base.metadata.create_all(engine)
        self.session = scoped_session(sessionmaker(bind=engine))
        return self.session

class Item():
    """Entity parsed from subscription"""
    def __init__(self, url, title, description):
        self.url = url
        self.title = title
        self.description = description

class SubscriptionType(Enum):
    """Type of subscription"""
    RSS     = 1
    """RSS feed."""

class UserState(Enum):
    """State of current user"""
    IDLE        = 1
    """Don't waiting any message"""
    SUBSCRIBE   = 2
    """Watiting for URL to subscribe"""
    UNSUBSCRIBE = 3
    """Watiting for URL unsubscribe from"""


