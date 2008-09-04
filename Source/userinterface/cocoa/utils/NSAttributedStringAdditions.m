//
//  NSAttributedStringAdditions.m
//  MacTierra
//
//  Created by Simon Fraser on 9/3/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "NSAttributedStringAdditions.h"


@implementation NSAttributedString(MTAttributedStringAdditions)

+ (id)attributedString
{
  return [[[NSAttributedString alloc] initWithString:@""] autorelease];
}

+ (id)attributedStringWithString:(NSString*)inString
{
  return [[[NSAttributedString alloc] initWithString:inString] autorelease];
}

+ (id)attributedStringWithString:(NSString*)inString attributes:(NSDictionary*)inAttributes
{
  return [[[NSAttributedString alloc] initWithString:inString attributes:inAttributes] autorelease];
}

@end


@implementation NSMutableAttributedString(MTMutableAttributedStringAdditions)

+ (id)attributedString
{
  return [[[NSMutableAttributedString alloc] initWithString:@""] autorelease];
}

+ (id)attributedStringWithString:(NSString*)inString attributes:(NSDictionary*)inAttributes
{
  return [[[NSMutableAttributedString alloc] initWithString:inString attributes:inAttributes] autorelease];
}

- (void)appendFormatWithAttributes:(NSDictionary*)inAttributes format:(NSString*)inFormat, ...
{
  va_list args;
  va_start(args, inFormat);
  
  NSString*   formattedString = [[NSString alloc] initWithFormat:inFormat arguments:args];

  va_end(args);

  [self appendAttributedString:[NSAttributedString attributedStringWithString:formattedString attributes:inAttributes]];
  
  [formattedString release];
}

- (void)appendFormatListWithAttributes:(NSDictionary*)inAttributes format:(NSString*)inFormat vaList:(va_list)inArgList
{
  NSString*   formattedString = [[NSString alloc] initWithFormat:inFormat arguments:inArgList];
  [self appendAttributedString:[NSAttributedString attributedStringWithString:formattedString attributes:inAttributes]];
  [formattedString release];
}

- (void)appendString:(NSString*)inString attributes:(NSDictionary*)inAttributes
{
  [self appendAttributedString:[NSAttributedString attributedStringWithString:inString attributes:inAttributes]];
}

- (void)empty
{
  [self deleteCharactersInRange:NSMakeRange(0, [self length])];
}

@end
