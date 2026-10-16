//
//  AuditID.h
//  BankTransfer
//
//  Created by Anussha on 16/10/26.
//

#ifndef AuditID_h
#define AuditID_h

#pragma once
#include <string>

// Each thread gets its own copy of this variable
inline thread_local std::string auditID = "none";

#endif /* AuditID_h */
