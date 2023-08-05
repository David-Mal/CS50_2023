-- Keep a log of any SQL queries you execute as you solve the mystery.
--1) the description of the crime
SELECT description FROM crime_scene_reports WHERE day = 28 AND month = 7 AND street = 'Humphrey Street';
--  the theft took place at 10:15
--  3 witnesses
--2) finding more about the witnesses
SELECT * FROM interviews WHERE month = 7 AND day = 28;
--  7 interviews were conducted on the day of the theft, 3 of them were about the theft
--  Ruth - the thief left the bakery's parking lot within 10 minutes of the theft
--  Eugene - the thief was at an ATM withdrawing some money on Leggett Street earlier that morning
--  Raymond - talked on the phone for less than a minute about taking the earliest flight out of
--  Fiftyville on June 29th. The thief was asking his complice to book the flight tickets.
--3) I searched the license plates of the cars that left between 10:15 and 10:25;
SELECT * FROM bakery_security_logs WHERE month = 7 AND day = 28 AND hour = 10 AND minute >= 15 AND minute <= 25 AND activity = 'exit';
--  8 cars, the id numbers are consecutive starting from 260
--  ('5P2BI95', '94KL13X', '6P58WS2', '4328GD8', 'G412CB7', 'L93JTIZ', '322W7JE', '0NTHK55')
--4) Checked people with the above license plate numbers
SELECT * FROM people WHERE license_plate IN ('5P2BI95', '94KL13X', '6P58WS2', '4328GD8', 'G412CB7', 'L93JTIZ', '322W7JE', '0NTHK55');
--5) Looking for people who used an ATM on Legget Street Iman Bruce Luca
SELECT * FROM people JOIN bank_accounts ON people.id = bank_accounts.person_id WHERE account_number IN (SELECT account_number  FROM atm_transactions WHERE month = 7 AND day = 28 AND atm_location = 'Leggett Street' AND transaction_type = 'withdraw');
--  Bruce, Iman and Luca where all on Leggett Street that morning withdrawing money from an ATM and they also left from
--  the bakery between 10:15 an 10:25
--6) These are the people who spoke on the phone for less than a minute on June 28th
SELECT name FROM people WHERE phone_number IN (SELECT caller FROM phone_calls WHERE month = 7 AND day = 28 AND duration < 60 AND caller IN (SELECT phone_number FROM people JOIN bank_accounts ON people.id = bank_accounts.person_id WHERE account_number IN (SELECT account_number  FROM atm_transactions WHERE month = 7 AND day = 28 AND atm_location = 'Leggett Street' AND transaction_type = 'withdraw')));
--  Bruce is the only one who matches all the hints gave by the witnesses so he is the theft
--7) Finding the person who Bruce called, the accomplice
SELECT name FROM people WHERE phone_number = (SELECT receiver FROM phone_calls WHERE caller = (SELECT phone_number FROM people WHERE name = 'Bruce') AND duration < 60 AND month = 7 AND day = 28);
--8) Finfing where Bruce escaped
SELECT people.name, people.passport_number, airports.city, airports.full_name FROM airports, flights, passengers, people WHERE airports.id = flights.destination_airport_id AND flights.id = passengers.flight_id AND passengers.passport_number = people.passport_number AND flights.origin_airport_id = (SELECT airports.id FROM airports WHERE city = "Fiftyville") AND day = 29 AND month = 07 AND year = 2021 ORDER BY flights.hour;
--  So this gives us the last answer, the plane which Bruce was on was heading to New York.