-- Add lib file and other libraries to path
package.path = './libs/?.lua;' .. package.path
package.path = './libs/?/init.lua;' .. package.path

package.path = './scripts/?.lua;' .. package.path
package.path = './scripts/?/init.lua;' .. package.path

package.path = './scripts/libs/?.lua;' .. package.path
package.path = './scripts/libs/?/init.lua;' .. package.path

-- Load penlight
require 'pl'