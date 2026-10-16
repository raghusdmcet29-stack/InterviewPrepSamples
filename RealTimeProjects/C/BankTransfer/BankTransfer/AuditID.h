//
//  AuditID.h
//  BankTransfer
//
//  Created by Anussha on 16/10/26.
//

#ifndef AuditID_h
#define AuditID_h
#pragma once

// Each thread gets its own copy of this variable
static _Thread_local char auditID[32] = "none";

#endif /* AuditID_h */
