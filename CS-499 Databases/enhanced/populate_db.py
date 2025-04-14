from faker import Faker
import random
from datetime import datetime
from AnimalShelter import AnimalShelter

# Setup

shelter = AnimalShelter(username='aacuser', password='password')

fake = Faker()

# Animal types and their breeds
animal_types = {
    "Dog": [
        "Labrador Retriever Mix", "Chesapeake Bay Retriever", "Newfoundland", "German Shepherd",
        "Golden Retriever", "Bloodhound", "Rottweiler", "Alaskan Malamute", "Siberian Husky"
    ],
    "Cat": [
        "Domestic Shorthair", "Siamese", "Maine Coon", "Persian", "Sphynx", "Bengal"
    ],
    "Rabbit": [
        "Mini Lop", "Dutch", "Flemish Giant", "English Spot"
    ],
    "Bird": [
        "Parakeet", "Canary", "Cockatiel", "Lovebird"
    ]
}

colors = ["Black", "White", "Brown", "Gray", "Orange", "Calico", "Tabby", "Spotted"]
sexes = ["Intact Male", "Intact Female", "Neutered Male", "Spayed Female"]
outcome_types = ["Adoption", "Transfer", "Return to Owner", "Relocation"]

# helper function to calculate age in weeks
def calculate_weeks(dob):
    return round((datetime.now().date() - dob).days / 7, 1)


# Create 300 random animals
for _ in range(300):
    animal_type = random.choice(list(animal_types.keys()))
    breed = random.choice(animal_types[animal_type])
    dob = fake.date_of_birth(minimum_age=0, maximum_age=10)
    age_weeks = calculate_weeks(dob)
    
    # Generate random data
    data = {
        "age_upon_outcome": f"{int(age_weeks / 52)} years" if age_weeks >= 52 else f"{int(age_weeks)} weeks",
        "age_upon_outcome_in_weeks": age_weeks,
        "animal_id": fake.unique.bothify(text='A######'),
        "animal_type": animal_type,
        "breed": breed,
        "color": random.choice(colors),
        "date_of_birth": dob.strftime("%Y-%m-%d"),
        "datetime": fake.date_time_this_year().strftime("%Y-%m-%d %H:%M:%S"),
        "monthyear": datetime.now().strftime("%Y-%m"),
        "name": fake.first_name(),
        "outcome_subtype": "",
        "outcome_type": random.choice(outcome_types),
        "sex_upon_outcome": random.choice(sexes),
        "location_lat": round(random.uniform(30.1, 30.9), 4),
        "location_long": round(random.uniform(-97.9, -97.0), 4)
    }

    # Create the animal
    shelter.create(data)

print("Created 300 diverse animal records successfully.")
