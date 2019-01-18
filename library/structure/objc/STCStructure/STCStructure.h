//Don't change this file manually, it was generated by automation tool.

#import <Foundation/Foundation.h>

typedef NS_ENUM(int, STCJSONType) {
    STCJSONTypeNull,
    STCJSONTypeBool,
    STCJSONTypeNumber,
    STCJSONTypeString,
    STCJSONTypeArray,
    STCJSONTypeObject,
};

@interface STCJSONValue : NSObject

+ (instancetype)fromString:(NSString *)string;
+ (instancetype)fromFile:(NSString *)path;
+ (instancetype)value;

- (NSString *)toString;
- (BOOL)toFile:(NSString *)path;

- (STCJSONType)type;

- (BOOL)boolValue;
- (int)intValue;
- (int64_t)int64Value;
- (float)floatValue;
- (double)doubleValue;
- (NSString *)stringValue;

- (int)arraySize;
- (void)addItem:(STCJSONValue *)item;
- (void)insertItem:(STCJSONValue *)item index:(int)index;
- (void)removeItemAt:(int)index;
- (STCJSONValue *)itemAt:(int)index;

- (NSArray<NSString *> *)objectNames;
- (void)setObject:(STCJSONValue *)object name:(NSString *)name;
- (void)removeObjectForName:(NSString *)name;
- (STCJSONValue *)objectForName:(NSString *)name;

@end
