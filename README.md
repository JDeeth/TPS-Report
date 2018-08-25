# TPS-Report

This will be an application to assist administrators of Blackbaud's The Raiser's Edge fundraising CRM system to check phone numbers in Raiser's Edge aginst the Ofcom Telephone Preference Service list.

It will generate .csv data to import back into Raiser's Edge to flag constituent records who match with the TPS list, in the form of a constituent attribute.

Process
-------

1. The user will create/locate an RE Query with the following criteria:
   - "Phone number" is one of the types of phone number which is actually a phone number, not an email address
   - other filtering criteria as desired by the user may be added
2. The output of this query shall contain:
   - Constituent name
   - Constituent ID
   - Phone number
3. The results of this query shall be exported directly from the query as a .csv file
4. The user runs this application, which will ask for that .csv file and the TPS list
5. The application will clean up the formatting of the RE phone numbers, discarding non-UK numbers and converting any +44 prefix to 0
6. The application will check each phone number for a match against the TPS list
7. When all the numbers are checked, the application will generate a .csv file containing data needed to add an attribute to each constituent with a conflicting phone number
8. The user uses this .csv file to add the attributes using RE's Import Constituent Attributes


