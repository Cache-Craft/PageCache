import random


def generate(kind, count, unique_pages):

    kind = kind.lower()

    if kind == "sequential":

        pages = [i % unique_pages for i in range(count)]
        locality = 0.0

    elif kind == "random":

        pages = [
            random.randrange(unique_pages)
            for _ in range(count)
        ]

        locality = 0.0

    elif kind == "locality":

        hot = max(1, unique_pages // 5)

        pages = [
            random.randrange(hot)
            if random.random() < 0.8
            else random.randrange(unique_pages)
            for _ in range(count)
        ]

        locality = 0.8

    else:
        raise ValueError(
            "type must be sequential, random, or locality"
        )

    return pages, locality