#include "IO.hh"

IO::DB::DB(char const *filename, int flags) {
    int result = sqlite3_open_v2(filename, &(this->db), flags, NULL);
    if (result) {
        spdlog::error("can't open database: {}", sqlite3_errmsg(db));
        this->valid = false;
        sqlite3_close(this->db);
    }
}

IO::DB::~DB() {
    for (int i = 0; i < this->stmtCsr; i++) {
        sqlite3_finalize(this->stmts[i]);
    }
    sqlite3_close(this->db);
}

bool IO::DB::getValid() {
    return this->valid;
}

sqlite3_stmt *IO::DB::prepare(char const *sql) {
    if (this->stmtCsr == IO::DB::MAX_STMTS) {
        spdlog::error("maximum statements on db connection reached");
        return NULL;
    }
    if (!this->valid) {
        spdlog::error("cannot prepare statement with invalid connection");
        return NULL;
    }
    int result = sqlite3_prepare_v2(
        this->db,
        sql,
        -1,
        this->stmts + this->stmtCsr,
        NULL
    );
    if (result) {
        spdlog::error(
            "error preparing statement({}): {}",
            result,
            sqlite3_errmsg(db)
        );
        return NULL;
    }
    this->stmtCsr++;
    return this->stmts[this->stmtCsr - 1];
}
