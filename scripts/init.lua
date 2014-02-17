-- Add lib file and other libraries to path
package.path = './libs/?.lua;' .. package.path
package.path = './libs/?/init.lua;' .. package.path

-- Load penlight
require 'pl'