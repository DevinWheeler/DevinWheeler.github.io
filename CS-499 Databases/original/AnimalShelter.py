from pymongo import MongoClient
from bson.objectid import ObjectId

class AnimalShelter(object):
    """ CRUD operations for Animal collection in MongoDB """

    def __init__(self, username, password, host='nv-desktop-services.apporto.com', port=30297, db='AAC', collection='animals'):
        # Initializing the MongoClient with dynamic username and password.
        self.client = MongoClient(f'mongodb://{username}:{password}@{host}:{port}')
        self.database = self.client[db]
        self.collection = self.database[collection]

    def create(self, data):
        # Method to create a new document in the collection
        if data:
            try:
                self.collection.insert_one(data)
                return True
            except Exception as e:
                print(f"An error occurred: {e}")
                return False
        else:
            raise ValueError("Data parameter is empty")

    def read(self, query):
       # Method to read documents from the collection
        try:
            cursor = self.collection.find(query)
            result = [doc for doc in cursor]
            return result
        except Exception as e:
            print(f"An error occurred: {e}")
            return []
            
    def update(self, query, update_data):
        # Method to update documents in the collection
        if query and update_data:
            try:
                result = self.collection.update_many(query, {'$set': update_data})
                return result.modified_count
            except Exception as e:
                print(f"An error occurred: {e}")
                return 0
        else:
            raise ValueError("Query and update_data parameters are required")

    def delete(self, query):
        # Method to delete documents from the collection
        if query:
            try:
                result = self.collection.delete_many(query)
                return result.deleted_count
            except Exception as e:
                print(f"An error occurred: {e}")
                return 0
        else:
            raise ValueError("Query parameter is required")
