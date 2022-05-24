#!/usr/bin/env python
# -*- coding: utf-8 -*-
import os

from sqlalchemy import Column, Integer, String, DateTime, ForeignKey, Table, Enum, func
from sqlalchemy.orm import relationship, backref
from sqlalchemy.ext.declarative import declarative_base

from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker, scoped_session

Base = declarative_base()

class Task(Base):
    """Info about task for current user."""
    __tablename__ = 'tasks'
    id = Column(Integer, primary_key=True)
    """Unique id. Fills automatically. """
    user_id = Column(Integer)
    """Owner's user id."""
    text = Column(String)
    """Task's text representation."""
        
    def __init__(self, user_id, text):
        self.user_id = user_id
        self.text = text

    def __repr__(self):
        return "<Task('%d','%s')>" % (self.user_id, self.text)



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

class UserState(Enum):
    """State uf current user."""
    IDLE     = 1
    """Don't waiting any message"""
    CREATE    = 2
    """Watiting for new task's text."""
    DELETE   = 3
    """Watiting for deleting task's text."""