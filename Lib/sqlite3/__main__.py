from argparse import ArgumentParser
from code import InteractiveConsole

import sqlite3
import sys


class SqliteInteractiveConsole(InteractiveConsole):

    def __init__(self, database):
        super().__init__()
        self.con = sqlite3.connect(database)
        self.cur = self.con.cursor()

    def runsql(self, sql):
        try:
            self.cur.execute(sql)
            res = self.cur.fetchall()
            if res:
                print(res)
        except sqlite3.Error as e:
            print(str(e))

    def runpy(self, source, filename='<input>', symbol='single'):
        code = self.compile(source, filename, symbol)
        self.runcode(code)

    def printhelp(self, ignored):
        print(
            'Enter SQL code and press enter. The REPL will run each command\n'
            'using execute() and then fetchall() on the database cursor. If\n'
            'the query produces any results, they are printed using\n'
            'print(str(result)).\n'
            '\n'
            f'Type "help", "copyright", "credits" or "license" for more '
            f'information.\n'
        )

    def runsource(self, source, filename='<input>', symbol='single'):
        keywords = {
            'version': lambda x: print(f'{sqlite3.sqlite_version}'),
            'help': self.printhelp,
            'copyright': self.runpy,
            'credits': self.runpy,
            'license': self.runpy,
            'license()': self.runpy,
            'quit()': self.runpy,
            'quit': self.runpy,
        }
        keywords.get(source, self.runsql)(source)


if __name__ == '__main__':
    parser = ArgumentParser(
        description='Python sqlite3 REPL',
        prog='python -m sqlite3'
    )
    parser.add_argument(
        '-f', '--filename',
        type=str, dest='database', action='store', default=':memory:',
        help='Database to open (default in-memory database)'
    )
    args = parser.parse_args()
    database = args.database

    if database == ':memory:':
        db_name = 'a transient in-memory database'
    else:
        db_name = f"'{database}'"
    banner = (
        f'Python {sys.version} on {sys.platform}\n'
        f'sqlite-{sqlite3.sqlite_version} (based on pysqlite '
        f'{sqlite3.version} REPL\n'
        f'Connected to {db_name}\n'
        f'Each command will be run using execute() on the cursor.\n'
        f'Type "help", "copyright", "credits" or "license" for more '
        f'information.\n'
    )
    sys.ps1 = 'sqlite> '

    console = SqliteInteractiveConsole(database)
    console.interact(banner, exitmsg='')
