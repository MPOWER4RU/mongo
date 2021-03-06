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

#include "mongo/base/disallow_copying.h"
#include "mongo/db/repl/storage_interface.h"
#include "mongo/stdx/mutex.h"

namespace mongo {
namespace repl {

class StorageInterfaceMock : public StorageInterface {
    MONGO_DISALLOW_COPYING(StorageInterfaceMock);

public:
    StorageInterfaceMock() = default;

    bool getInitialSyncFlag(OperationContext* txn) const override;
    void setInitialSyncFlag(OperationContext* txn) override;
    void clearInitialSyncFlag(OperationContext* txn) override;

    BatchBoundaries getMinValid(OperationContext* txn) const override;
    void setMinValid(OperationContext* txn,
                     const OpTime& endOpTime,
                     const DurableRequirement durReq) override;
    void setMinValid(OperationContext* txn, const BatchBoundaries& boundaries) override;


    StatusWith<OpTime> writeOpsToOplog(OperationContext* txn,
                                       const NamespaceString& nss,
                                       const MultiApplier::Operations& operations) override;

    MultiApplier::Operations getOperationsWrittenToOplog() const;

private:
    mutable stdx::mutex _initialSyncFlagMutex;
    bool _initialSyncFlag = false;

    mutable stdx::mutex _minValidBoundariesMutex;
    BatchBoundaries _minValidBoundaries = {OpTime(), OpTime()};

    mutable stdx::mutex _operationsWrittenToOplogMutex;
    MultiApplier::Operations _operationsWrittenToOplog;
};

}  // namespace repl
}  // namespace mongo
