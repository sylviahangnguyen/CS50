/* list the titles of the five highest rated movies (in order)
that Chadwick Boseman starred in,
starting with the highest rated. */

SELECT title FROM movies AS m
JOIN ratings AS r
ON m.id = r.movie_id
WHERE id IN
(
    SELECT movie_id FROM stars
    WHERE person_id IN
    (
        SELECT id FROM people
        WHERE name = "Chadwick Boseman"
    )
)

ORDER BY r.rating DESC
LIMIT 5;