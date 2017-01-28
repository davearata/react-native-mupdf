#include "common.h"
#import "MuDocumentController.h"
#import "MuPdfPlugin.h"
#import "RCTReactNativeMupdf.h"
#import "RCTEventDispatcher.h"

@implementation ReactNativeMupdf
{
  MuDocRef *doc;
  char *_filePath;
}

@synthesize bridge = _bridge;

enum
{
    // use at most 128M for resource cache
    ResourceCacheMaxSize = 128<<20	// use at most 128M for resource cache
};

RCT_EXPORT_MODULE();

queue = dispatch_queue_create("com.artifex.mupdf.queue", NULL);

ctx = fz_new_context(NULL, NULL, ResourceCacheMaxSize);
fz_register_document_handlers(ctx);

RCT_REMAP_METHOD(openPdf:(NSString *)path documentTitle:(NSString *)documentTitle options:(NSString *)options,
                 resolver:(RCTPromiseResolveBlock)resolve
                 rejecter:(RCTPromiseRejectBlock)reject)
{
  if (path != nil && [path length] > 0) {
    [self openDocument:path title:documentTitle options:options];
  } else {
    reject(@"invalid_path", @"There was no path specified")
  }
}

- (void) openDocument: (NSString*)nspath title:(NSString*)documentTitle options:(NSDictionary*)options
{
  // _filePath = malloc(strlen([nspath UTF8String])+1);
  // if (_filePath == NULL) {
    // printf("Out of memory in openDocument");
    // return;
  // }

  // strcpy(_filePath, [nspath UTF8String]);

  // dispatch_sync(queue, ^{});

  // printf("open document '%s'\n", _filePath);

  doc = [[MuDocRef alloc] initWithFilename:nspath];
  if (!doc) {
    printf("Cannot open document");
    return;
  }

  MuDocumentController *document = [[MuDocumentController alloc] initWithFilename: documentTitle path:nspath document:doc options:options];
  if (document) {
    UINavigationController* navigationController = [[UINavigationController alloc] initWithRootViewController:document];
    [[NSNotificationCenter defaultCenter] addObserver:self
                                          selector:@selector(didDismissDocumentController:)
                                          name:@"DocumentControllerDismissed"
                                          object:nil];
    [self.viewController presentViewController:navigationController animated:YES completion:nil];
  }
  free(_filePath);
}

-(void)didDismissDocumentController:(NSNotification *)notification {
  NSDictionary* saveResults = [notification object];
  [self.bridge.eventDispatcher sendAppEventWithName:@"PdfSaved"
                                               body:saveResults]
}

@end
