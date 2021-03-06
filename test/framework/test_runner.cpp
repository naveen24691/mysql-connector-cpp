/*
Copyright (c) 2008, 2011, Oracle and/or its affiliates. All rights reserved.

The MySQL Connector/C++ is licensed under the terms of the GPLv2
<http://www.gnu.org/licenses/old-licenses/gpl-2.0.html>, like most
MySQL Connectors. There are special exceptions to the terms and
conditions of the GPLv2 as it is applied to this software, see the
FLOSS License Exception
<http://www.mysql.com/about/legal/licensing/foss-exception.html>.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published
by the Free Software Foundation; version 2 of the License.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin St, Fifth Floor, Boston, MA 02110-1301  USA
*/



#include "test_runner.h"
#include "test_factory.h"
#include "test_case.h"
#include "test_listener.h"
#include "test_filter.h"


namespace testsuite
{

TestsRunner::TestsRunner()
  : startOptions( NULL  )
  , filter      ( NULL  )
{
}


bool TestsRunner::runTests()
{
  TestSuiteNames.empty();

  TestSuiteFactory::theInstance().getTestsList( TestSuiteNames );

  TestsListener::setVerbose(startOptions->getBool( "verbose") );
  TestsListener::doTiming(  startOptions->getBool( "timer"  ) );

  String dummy;

  for ( constStrListCit cit= TestSuiteNames.begin(); cit != TestSuiteNames.end(); ++cit )
  {
    dummy= *cit;
    dummy+= "::"; // to be caught by filters like "!SuiteName::*"

    if ( Admits( dummy ) )
    {
      Test * ts= TestSuiteFactory::theInstance().createTest( *cit );
      ts->runTest();
    }
    //else TODO: Add skipping by filter condition message
  }

  TestsListener::theInstance().summary();

  return TestsListener::allTestsPassed();
}


void TestsRunner::setStartOptions(StartOptions * options)
{
  startOptions=options;
}


void TestsRunner::setTestsFilter  ( Filter & _filter )
{
  filter= &_filter;
}


StartOptions * TestsRunner::getStartOptions()
{
  return theInstance().startOptions;
}


bool TestsRunner::Admits( const String & testName )
{
  if ( theInstance().filter != NULL )
    return theInstance().filter->Admits( testName );

  return true;
}

}
