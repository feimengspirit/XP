//
//  XPTests.m
//  XPTests
//
//  Created by mengfei.mf on 5/28/15.
//  Copyright (c) 2015 mengfei.mf. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <XCTest/XCTest.h>
#import "XP.h"

using namespace feimengspirit;

@interface XPTests : XCTestCase

@end

class Base
{
public:
    virtual void test() = 0;
    virtual ~Base()
    {
        printf("deconstructored!\n");
    }

    int _i;
};

class Derive : public Base
{
public:
    void test()override
    {
        printf("hello\n");
    }
};


XP<Base> func()
{
    return new Derive;
}

@implementation XPTests

- (void)setUp {
    [super setUp];
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
}

- (void)testExample {
    for (int i = 0; i < 1; ++i) {
        XP<Base> bXP(new Derive);
        XP<Base> aXP = bXP;
        XP<Base> cXP(aXP);
        XP<Base> dXP(bXP);
        XP<Derive> eXP(bXP);

        XP<Base> fXP = new Derive;
        XP<Base> gXP = fXP;
        XP<Base> hXP(eXP);

        aXP->test();
        printf("i: %d\n", aXP->_i);
        bXP->test();
        cXP->test();
        dXP->test();
        eXP->test();

        fXP->test();
        gXP->test();
        hXP->test();

        XP<int> iXP = new int(0);

        XP<Base> jXP(new Derive[10], [](Base *p){ delete []p; });

        XP<Base> kXP = func();
        kXP = aXP;
        kXP = new Derive;
    }
}

- (void)testPerformanceExample {
    // This is an example of a performance test case.
    [self measureBlock:^{
        // Put the code you want to measure the time of here.
    }];
}

@end
