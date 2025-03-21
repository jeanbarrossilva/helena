// Copyright © 2025 Jean Silva
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
// in compliance with the License. You may obtain a copy of the License at
//
//                            http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed under the License
// is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
// or implied. See the License for the specific language governing permissions and limitations under
// the License.

use crate::node::{Node, UnmatchedPatternError};

/// Description of a declaration of a parameter to be passed into a function as a value.
#[derive(Debug)]
struct ValueParameterDeclaration<'a> {
  /// Name of the type of the value as it was input by the user (may or may not be qualified).
  type_name: &'a str,

  /// Name attributed to this value parameter.
  identifier: &'a str
}

impl<'a> Node<'a> {
  /// Denotes that a declaration of a function is expected to follow this node.
  fn branch_to_function_declaration(
    self,
    identifier: &'a str,
    value_parameters: &'a [ValueParameterDeclaration<'a>]
  ) -> Result<Self, UnmatchedPatternError> {
    self.branch_to("func", |node| {
      node.branch_to_space(|node| {
        node.branch_to_identifier(identifier, |node| {
          node.branch_to("(", |node| {
            value_parameters.iter().fold(
              node.branch_to(")", |node| node.branch_to(":", |node| node.leaf())),
              |value_parameter_node, value_parameter| {
                value_parameter_node.and_then(|node| {
                  node.branch_to_identifier(value_parameter.type_name, |node| {
                    node.branch_to_space(|node| {
                      node
                        .branch_to_identifier(value_parameter.identifier, |node| Ok(node))
                        .and_then(|node| node.leaf())
                    })
                  })
                })
              }
            )
          })
        })
      })
    })
  }
}

#[cfg(test)]
mod tests {
  use crate::{function::function::ValueParameterDeclaration, node::Node};

  #[test]
  fn branches_to_a_function_declaration() {
    assert!(Node::default()
      .branch_to_function_declaration(
        "main",
        &[ValueParameterDeclaration {
          type_name:  "string[]",
          identifier: "args"
        }]
      )
      .is_ok())
  }
}
