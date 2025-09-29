CREATE STREAM person_knows_person1(person1_id long, person2_id long) FROM FILE 'person_knows_person1.dat' LINE DELIMITED CSV;
CREATE STREAM person_knows_person2(person1_id long, person2_id long) FROM FILE 'person_knows_person2.dat' LINE DELIMITED CSV;
CREATE STREAM person_has_interest_tag(person_id long, tag_id long) FROM FILE 'person_has_interest_tag.dat' LINE DELIMITED CSV;

SELECT COUNT(*)
FROM person_knows_person1 AS pkp1, person_knows_person2 AS pkp2, person_has_interest_tag AS phit
WHERE pkp1.person2_id = pkp2.person1_id AND pkp2.person2_id = phit.person_id;