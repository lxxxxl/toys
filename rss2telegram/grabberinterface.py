#!/usr/bin/env python
# -*- coding: utf-8 -*-

from utils import Subscription, Item

class GrabberInterface:
    """Interface for grabbers"""
    def process(self, subscription: Subscription) -> dict:
        """Process grabbing of subscription.

        Keyword arguments:
        subscription -- grabbed subscriotion

        Keyword returns:
        array of Item objects
        """
        pass

