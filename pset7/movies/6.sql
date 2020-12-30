/* determine the average rating of all movies released in 2012 */

SELECT AVG(rating) FROM
    (SELECT * FROM movies AS m
     JOIN ratings AS r
     ON m.id = r.movie_id
     WHERE m.year = 2012);