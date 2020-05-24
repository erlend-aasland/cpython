import sqlite3

# Example taken from https://www.sqlite.org/windowfunctions.html#udfwinfunc
class WindowSumInt:
    def __init__(self):
        self.count = 0

    def step(self, value):
        self.count += value

    def value(self):
        return self.count

    def inverse(self, value):
        self.count -= value

    def finalize(self):
        return self.count

con = sqlite3.connect(':memory:')
cur = con.cursor()
cur.execute('create table test(x, y)')
cur.execute("insert into test values('a', 4)")
cur.execute("insert into test values('b', 5)")
cur.execute("insert into test values('c', 3)")
cur.execute("insert into test values('d', 8)")
cur.execute("insert into test values('e', 1)")
con.create_window_function('sumint', 1, WindowSumInt)
cur.execute("""
    select x, sumint(y) over (
        order by x rows between 1 preceding and 1 following
    ) as sum_y
    from test order by x
""")
print(cur.fetchall())
con.close()
