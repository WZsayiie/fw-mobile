//Don't change this file manually, it was generated by automation tool.

#import "CQFoundation.h"
#import "CQBridge.h"

CQ_C_LINK void CQ_Logger_info(CQBridgeValue message, CQBridgeValue file, CQBridgeValue line) {
    NSString *aa = CQBridgeGetNSString(message);
    NSString *bb = CQBridgeGetNSString(file);
    int32_t cc = CQBridgeGetInt32(line);
    [CQLogger info:aa file:bb line:cc];
}

CQ_C_LINK void CQ_Logger_error(CQBridgeValue message, CQBridgeValue file, CQBridgeValue line) {
    NSString *aa = CQBridgeGetNSString(message);
    NSString *bb = CQBridgeGetNSString(file);
    int32_t cc = CQBridgeGetInt32(line);
    [CQLogger error:aa file:bb line:cc];
}
