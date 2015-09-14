/**
 *    Copyright (C) 2015 MongoDB Inc.
 *
 *    This program is free software: you can redistribute it and/or  modify
 *    it under the terms of the GNU Affero General Public License, version 3,
 *    as published by the Free Software Foundation.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU Affero General Public License for more details.
 *
 *    You should have received a copy of the GNU Affero General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *    As a special exception, the copyright holders give permission to link the
 *    code of portions of this program with the OpenSSL library under certain
 *    conditions as described in each individual source file and distribute
 *    linked combinations including the program with the OpenSSL library. You
 *    must comply with the GNU Affero General Public License in all respects for
 *    all of the code used other than as permitted herein. If you modify file(s)
 *    with this exception, you may extend this exception to your version of the
 *    file(s), but you are not obligated to do so. If you do not wish to do so,
 *    delete this exception statement from your version. If you delete this
 *    exception statement from all source files in the program, then also delete
 *    it in the license file.
 */

#pragma once

#include "mongo/db/repl/optime.h"

namespace mongo {

class BSONObj;
class BSONObjBuilder;

namespace rpc {

/**
 * This class encapsulates the response that mongod will return to mongos on every
 * command, containing metadata information about the config servers.
 */
class ConfigServerResponseMetadata {
public:
    explicit ConfigServerResponseMetadata(repl::OpTime opTime);

    /**
     * format:
     * configsvr: {
     *     opTime: {ts: Timestamp(0, 0), t: 0}
     * }
     */
    static StatusWith<ConfigServerResponseMetadata> readFromMetadata(const BSONObj& doc);
    void writeToMetadata(BSONObjBuilder* builder) const;

    /**
     * Returns the OpTime of the most recent operation on the config servers that this
     * shard has seen.
     */
    repl::OpTime getOpTime() const {
        return _opTime;
    }

private:
    repl::OpTime _opTime;
};

}  // namespace rpc
}  // namespace mongo