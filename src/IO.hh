#ifndef IO_H
#define IO_H

#include <spdlog/spdlog.h>
#include <glm/vec2.hpp>
#include <sqlite3.h>

/**
 * Contains functionality to do with file reading and writing that doesn't have
 * a better place to go.
 */
namespace IO {
    static int const FILE_IMAGE = 0;
    static int const FILE_MUSIC = 1;
    static int const FILE_SOUND = 2;
    static int const FILE_FRAG = 3;
    static int const FILE_VERT = 4;
    static int const FILE_SCRIPT = 5;

    /**
     * Represents a connection to a game database.
     */
    class DB {
        public:
            static int const MAX_STMTS = 64;

            /**
             * Initialises the database connection and sets the valid flag
             * which can be accessed with getValid, which tells you if the
             * connection is usable or not.
             * @param filename is the filename of the database file to
             *        connect to.
             * @param flags is the set of sqlite flags to apply.
             */
            DB(char const *filename, int flags);

            /**
             * Closes the db connection RAII style baby.
             */
            ~DB();

            /**
             * Tells you if the connection is actually open or not.
             * @return true iff the connection is open and working.
             */
            bool getValid();

            /**
             * Creates an sqlite prepared statement which you can then run
             * stuff on yourself. The statement's lifetime is managed by this
             * object which means it is only valid for as long as this object
             * exists, so you should really only store one of these in the same
             * scope as the db object but if you don't that's your problem I'm
             * not gonna tell you what to do.
             * @param sql is the sql text to turn into a statement.
             * @return created sqlite statement, or NULL if there was an error.
             */
            sqlite3_stmt *prepare(char const *sql);

            /**
             * Runs the given prepared statement and executes the given
             * callback.
             * @param sql is the sql code 
             * @param callback is the lambda function to call for each item. It
             *        gets no arguments because you can just capture the
             *        statement object to get the info.
             * @return true if everything was fine, and false if there was some
             *         kind of error (which gets logged).
             */
            template <typename F> bool exec(sqlite3_stmt *stmt, F callback) {
                while (true) {
                    int result = sqlite3_step(stmt);
                    switch (result) {
                        case SQLITE_ROW: callback();
                        case SQLITE_BUSY: continue;
                        case SQLITE_DONE: return true;
                    }
                    spdlog::error(
                        "error executing statement({}): {}",
                        result,
                        sqlite3_errmsg(this->db)
                    );
                    return false;
                }
            };

        private:
            bool valid = true;
            sqlite3 *db;
            sqlite3_stmt *stmts[DB::MAX_STMTS];
            int stmtCsr = 0;
    };

    /**
     * Gets the data from a file in the db and returns it.
     * @param db is the database connection to load with.
     * @param label is the name of the file which is how it's found.
     * @param callback is a callback to call with the data which should take
     *        a void const pointer and a size_t size.
     * @return true if all went well.
     */
    template <typename F> bool fileData(
        DB &db,
        char const *label,
        F callback
    ) {
        sqlite3_stmt *query = db.prepare(
            "SELECT data FROM file WHERE label = ?"
        );
        if (!query) return false;
        sqlite3_bind_text(query, 1, label, -1, SQLITE_STATIC);
        return db.exec(query, [query, callback]() {
            void const *data = sqlite3_column_blob(query, 0);
            int size = sqlite3_column_bytes(query, 0);
            callback(data, size);
        });
    }
};

#endif
