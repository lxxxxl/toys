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
    """Info about RSS or other source and owner's user id."""
    __tablename__ = 'subscriptions'
    id = Column(Integer, primary_key=True)
    """Unique id. Fills automatically. """
    user_id = Column(Integer)
    """Owner's user id."""
    url = Column(String)
    """URL with RSS or other feed."""
    type = Column(Integer)
    """Type of subscription, check enum SubscriptionType"""
    title = Column(String)
    """Subscription title."""
    lastupdate = Column(DateTime)
    """Date and time of last update."""
    items = relationship("Item", back_populates="subscription")
    """Items collected from this subscription."""
        
    def __init__(self, user_id, url, type, title, lastupdate):
        self.user_id = user_id
        self.url = url
        self.type = type
        self.title = title
        self.lastupdate = lastupdate

    def __repr__(self):
        return "<Subscription('%d','%s')>" % (self.user_id, self.url)

class Item(Base):
    """Downloaded item."""
    __tablename__ = 'items'
    id = Column(Integer, primary_key=True)
    """Unique id. Fills automatically. """
    subscription_id = Column(Integer, ForeignKey('subscriptions.id'))
    subscription = relationship("Subscription", back_populates="items")
    """Subscription that belongs item to"""
    url = Column(String)
    """Webpage URL."""
    title = Column(String)
    """Item title."""
    description = Column(String)
    """Item description."""
    datetime = Column(DateTime)
    """Publication date and time."""
    status = Column(Integer)
    """Item status, check enum ItemStatus"""
        
    def __init__(self, subscription, url, title, description, datetime, status):
        self.subscription = subscription
        self.url = url
        self.title = title
        self.description = description
        self.datetime = datetime
        self.status = status

    def __repr__(self):
        return "<Item('%d','%s')>" % (self.subscription, self.url)

class ItemStatus(Enum):
    """Collected item status."""
    NEW     = 1
    """Item just collected"""
    READ    = 2
    """Item was read."""
    SAVED   = 3
    """Item was saved."""

class SubscriptionType(Enum):
    """Type of subscription."""
    RSS     = 1
    """RSS feed."""

class Database(object):
    """Performs connection to database."""
    dbfilename = 'database.db'
    """Database filename."""
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
